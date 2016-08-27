/*
 * Copyright (C) 2013
 * Computer Graphics Group, University of Siegen
 * Written by Martin Lambers <martin.lambers@uni-siegen.de>
 * All rights reserved.
 */

#include <limits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include <GL/glew.h>

#include "lodepng.h"
#ifdef HAVE_GTA
# include <gta/gta.hpp>
#endif

#include "texload.hpp"


static void img_reverse_y(unsigned int height, unsigned int line_size, unsigned char* data)
{
    std::vector<unsigned char> tmp_line(line_size);
    for (unsigned int y = 0; y < height / 2; y++) {
        size_t ty = height - 1 - y;
        std::memcpy(&(tmp_line[0]), &(data[ty * line_size]), line_size);
        std::memcpy(&(data[ty * line_size]), &(data[y * line_size]), line_size);
        std::memcpy(&(data[y * line_size]), &(tmp_line[0]), line_size);
    }
}

bool load_png(GLenum target, const std::string& filename, bool reverse_y)
{
    std::vector<unsigned char> image;
    unsigned int width, height;

    unsigned error = lodepng::decode(image, width, height, filename);
    if (error) {
        fprintf(stderr, "png decoder error %d: %s\n", error, lodepng_error_text(error));
        return false;
    }

    if (reverse_y)
        img_reverse_y(height, 4 * width * sizeof(unsigned char), &(image[0]));

    GLint ua_bak;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &ua_bak);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(target, 0, GL_RGBA8, width, height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ua_bak);
    return true;
}

bool save_png(GLenum target, const std::string& filename, bool reverse_y)
{
    std::vector<unsigned char> image;
    int width, height;

    GLint pa_bak;
    glGetIntegerv(GL_PACK_ALIGNMENT, &pa_bak);
    glGetTexLevelParameteriv(target, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(target, 0, GL_TEXTURE_HEIGHT, &height);
    assert(width >= 1 && height >= 1);
    image.resize(4 * width * height * sizeof(unsigned char));
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glGetTexImage(target, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
    glPixelStorei(GL_PACK_ALIGNMENT, pa_bak);

    if (reverse_y)
        img_reverse_y(height, 4 * width * sizeof(unsigned char), &(image[0]));

    unsigned error = lodepng::encode(filename, image, width, height);
    if (error) {
        fprintf(stderr, "png encoder error %d: %s\n", error, lodepng_error_text(error));
        return false;
    }
    return true;
}

#ifdef HAVE_GTA

bool load_gta(GLenum target, const std::string& filename, bool reverse_y)
{
    FILE* f = std::fopen(filename.c_str(), "rb");
    if (!f)
        return false;

    gta::header hdr;
    try {
        hdr.read_from(f);
    }
    catch (std::exception& e) {
        return false;
    }
    if (hdr.dimensions() != 2 || hdr.components() < 1 || hdr.components() > 4
            || (hdr.component_type(0) != gta::int8
                && hdr.component_type(0) != gta::uint8
                && hdr.component_type(0) != gta::int16
                && hdr.component_type(0) != gta::uint16
                && hdr.component_type(0) != gta::int32
                && hdr.component_type(0) != gta::uint32
                && hdr.component_type(0) != gta::float32)
            || (hdr.components() > 1 && hdr.component_type(1) != hdr.component_type(0))
            || (hdr.components() > 2 && hdr.component_type(2) != hdr.component_type(0))
            || (hdr.components() > 3 && hdr.component_type(3) != hdr.component_type(0))
            || hdr.data_size() > std::numeric_limits<size_t>::max()) {
        return false;
    }

    std::vector<unsigned char> data(hdr.data_size());
    try {
        hdr.read_data(f, &(data[0]));
    }
    catch (std::exception& e) {
        return false;
    }

    int c = hdr.components();
    gta::type t = hdr.component_type(0);

    // TODO: here we assume the GTAs always contain channels in the order R, G, B, A.

    GLenum format = (c == 1 ? GL_RED : c == 2 ? GL_RG : c == 3 ? GL_RGB : GL_RGBA);
    GLenum type =
          t == gta::int8   ? GL_BYTE
        : t == gta::uint8  ? GL_UNSIGNED_BYTE
        : t == gta::int16  ? GL_SHORT
        : t == gta::uint16 ? GL_UNSIGNED_SHORT
        : t == gta::int32  ? GL_INT
        : t == gta::uint32 ? GL_UNSIGNED_INT
        : GL_FLOAT;
    GLenum internal_format = GL_RGB8;
    if (t == gta::int8) {
        internal_format = (c == 1 ? GL_R8I : c == 2 ? GL_RG8I : c == 3 ? GL_RGB8I : GL_RGBA8I);
    } else if (t == gta::uint8) {
        internal_format = (c == 1 ? GL_R8 : c == 2 ? GL_RG8 : c == 3 ? GL_RGB8 : GL_RGBA8);
        /* TODO: use SRGB color space where applicable? */
#if 0
        if (c == 3 || c == 4) {
            // check for SRGB color space
            const char* tagval = hdr.component_taglist(0).get("INTERPRETATION");
            if (tagval && std::strcmp(tagval, "SRGB/RED") == 0) {
                internal_format = (c == 3 ? GL_SRGB : GL_SRGB_ALPHA);
            }
        }
#endif
    } else if (t == gta::int16) {
        internal_format = (c == 1 ? GL_R16I : c == 2 ? GL_RG16I : c == 3 ? GL_RGB16I : GL_RGBA16I);
    } else if (t == gta::uint16) {
        internal_format = (c == 1 ? GL_R16UI : c == 2 ? GL_RG16UI : c == 3 ? GL_RGB16UI : GL_RGBA16UI);
    } else if (t == gta::int32) {
        internal_format = (c == 1 ? GL_R32I : c == 2 ? GL_RG32I : c == 3 ? GL_RGB32I : GL_RGBA32I);
    } else if (t == gta::uint32) {
        internal_format = (c == 1 ? GL_R32UI : c == 2 ? GL_RG32UI : c == 3 ? GL_RGB32UI : GL_RGBA32UI);
    } else if (t == gta::float32) {
        internal_format = (c == 1 ? GL_R32F : c == 2 ? GL_RG32F : c == 3 ? GL_RGB32F : GL_RGBA32F);
    }

    size_t line_size = hdr.element_size() * hdr.dimension_size(0);
    if (reverse_y)
        img_reverse_y(hdr.dimension_size(1), line_size, &(data[0]));

    GLint ua_bak;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &ua_bak);
    if (line_size % 4 == 0)
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    else if (line_size % 2 == 0)
        glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    else
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(target, 0, internal_format,
            hdr.dimension_size(0), hdr.dimension_size(1), 0,
            format, type, &(data[0]));
    glPixelStorei(GL_UNPACK_ALIGNMENT, ua_bak);
    std::fclose(f);
    return true;
}

bool save_gta(GLenum target, const std::string& filename, bool reverse_y)
{
    FILE* f = std::fopen(filename.c_str(), "wb");
    if (!f)
        return false;

    // The following assumes that all textures are 2D!

    GLint w, h, internal_format;
    glGetTexLevelParameteriv(target, 0, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(target, 0, GL_TEXTURE_HEIGHT, &h);
    glGetTexLevelParameteriv(target, 0, GL_TEXTURE_INTERNAL_FORMAT, &internal_format);
    assert(w >= 1 && h >= 1);

    int components = 0;
    switch (internal_format) {
    case GL_DEPTH_COMPONENT:
    case GL_LUMINANCE:
    case GL_RED:
    case GL_R8:
    case GL_R8_SNORM:
    case GL_R16:
    case GL_R16_SNORM:
    case GL_R16F:
    case GL_R32F:
    case GL_R8I:
    case GL_R8UI:
    case GL_R16I:
    case GL_R16UI:
    case GL_R32I:
    case GL_R32UI:
        components = 1;
        break;
    case GL_LUMINANCE_ALPHA:
    case GL_RG:
    case GL_RG8:
    case GL_RG8_SNORM:
    case GL_RG16:
    case GL_RG16_SNORM:
    case GL_RG16F:
    case GL_RG32F:
    case GL_RG8I:
    case GL_RG8UI:
    case GL_RG16I:
    case GL_RG16UI:
    case GL_RG32I:
    case GL_RG32UI:
        components = 2;
        break;
    case GL_R3_G3_B2:
    case GL_RGB4:
    case GL_RGB5:
    case GL_RGB10:
    case GL_RGB12:
    case GL_SRGB:
    case GL_SRGB8:
    case GL_R11F_G11F_B10F:
    case GL_RGB9_E5:
    case GL_RGB:
    case GL_RGB8:
    case GL_RGB8_SNORM:
    case GL_RGB16:
    case GL_RGB16_SNORM:
    case GL_RGB16F:
    case GL_RGB32F:
    case GL_RGB8I:
    case GL_RGB8UI:
    case GL_RGB16I:
    case GL_RGB16UI:
    case GL_RGB32I:
    case GL_RGB32UI:
        components = 3;
        break;
    case GL_RGBA4:
    case GL_RGB5_A1:
    case GL_RGB10_A2:
#ifdef GL_RGB10_A2UI
    case GL_RGB10_A2UI:
#endif
    case GL_RGBA12:
    case GL_SRGB_ALPHA:
    case GL_SRGB8_ALPHA8:
    case GL_RGBA:
    case GL_RGBA8:
    case GL_RGBA8_SNORM:
    case GL_RGBA16:
    case GL_RGBA16_SNORM:
    case GL_RGBA16F:
    case GL_RGBA32F:
    case GL_RGBA8I:
    case GL_RGBA8UI:
    case GL_RGBA16I:
    case GL_RGBA16UI:
    case GL_RGBA32I:
    case GL_RGBA32UI:
        components = 4;
        break;
    }
    assert(components > 0);

    gta::type gta_type = gta::blob;
    switch (internal_format) {
    case GL_LUMINANCE:
    case GL_RED:
    case GL_R8:
    case GL_R8_SNORM:
    case GL_LUMINANCE_ALPHA:
    case GL_RG:
    case GL_RG8:
    case GL_RG8_SNORM:
    case GL_R3_G3_B2:
    case GL_RGB4:
    case GL_RGB5:
    case GL_RGB10:
    case GL_RGB12:
    case GL_SRGB:
    case GL_SRGB8:
    case GL_RGB:
    case GL_RGB8:
    case GL_RGB8_SNORM:
    case GL_RGBA4:
    case GL_RGB5_A1:
    case GL_SRGB_ALPHA:
    case GL_SRGB8_ALPHA8:
    case GL_RGBA:
    case GL_RGBA8:
    case GL_RGBA8_SNORM:
    case GL_R8UI:
    case GL_RG8UI:
    case GL_RGB8UI:
    case GL_RGBA8UI:
        gta_type = gta::uint8;
        break;
    case GL_R8I:
    case GL_RG8I:
    case GL_RGB8I:
    case GL_RGBA8I:
        gta_type = gta::int8;
        break;
    case GL_DEPTH_COMPONENT:
    case GL_R16:
    case GL_R16_SNORM:
    case GL_R16F:
    case GL_R32F:
    case GL_RG16:
    case GL_RG16_SNORM:
    case GL_RG16F:
    case GL_RG32F:
    case GL_R11F_G11F_B10F:
    case GL_RGB9_E5:
    case GL_RGB16:
    case GL_RGB16_SNORM:
    case GL_RGB16F:
    case GL_RGB32F:
    case GL_RGBA16:
    case GL_RGBA16_SNORM:
    case GL_RGBA16F:
    case GL_RGBA32F:
    case GL_RGB10_A2:
#ifdef GL_RGB10_A2UI
    case GL_RGB10_A2UI:
#endif
    case GL_RGBA12:
        gta_type = gta::float32;
        break;
    case GL_R16I:
    case GL_RG16I:
    case GL_RGB16I:
    case GL_RGBA16I:
        gta_type = gta::int16;
        break;
    case GL_R16UI:
    case GL_RG16UI:
    case GL_RGB16UI:
    case GL_RGBA16UI:
        gta_type = gta::uint16;
        break;
    case GL_R32I:
    case GL_RG32I:
    case GL_RGB32I:
    case GL_RGBA32I:
        gta_type = gta::int32;
        break;
    case GL_R32UI:
    case GL_RG32UI:
    case GL_RGB32UI:
    case GL_RGBA32UI:
        gta_type = gta::uint32;
        break;
    }
    assert(gta_type != gta::blob); // make sure valid type is found
    gta::type gta_types[components];
    for (int i = 0; i < components; i++)
        gta_types[i] = gta_type;

    gta::header hdr;
    try {
        hdr.set_compression(gta::zlib9);
        hdr.set_dimensions(w, h);
        hdr.set_components(components, gta_types);
        if (internal_format == GL_SRGB || internal_format == GL_SRGB8
                || internal_format == GL_SRGB_ALPHA || internal_format == GL_SRGB8_ALPHA8) {
            hdr.component_taglist(0).set("INTERPRETATION", "SRGB/RED");
            hdr.component_taglist(1).set("INTERPRETATION", "SRGB/GREEN");
            hdr.component_taglist(2).set("INTERPRETATION", "SRGB/BLUE");
        }
        if (internal_format == GL_SRGB_ALPHA || internal_format == GL_SRGB8_ALPHA8) {
            hdr.component_taglist(3).set("INTERPRETATION", "ALPHA");
        }
        // don't set INTERPRETATION for other internal formats, since we don't
        // know if the data really represents colors (could be e.g. normals or vectors
        // or strange sensor data)
    }
    catch (std::exception& e) {
        return false;
    }

    std::vector<unsigned char> data(hdr.data_size());

    GLenum format = (internal_format == GL_DEPTH_COMPONENT ? GL_DEPTH_COMPONENT
            : components == 1 ? GL_RED
            : components == 2 ? GL_RG
            : components == 3 ? GL_RGB
            : GL_RGBA);
    GLenum type =
        gta_type == gta::int8   ? GL_BYTE
        : gta_type == gta::uint8  ? GL_UNSIGNED_BYTE
        : gta_type == gta::int16  ? GL_SHORT
        : gta_type == gta::uint16 ? GL_UNSIGNED_SHORT
        : gta_type == gta::int32  ? GL_INT
        : gta_type == gta::uint32 ? GL_UNSIGNED_INT
        : GL_FLOAT;
    GLint pa_bak;
    glGetIntegerv(GL_PACK_ALIGNMENT, &pa_bak);
    size_t line_size = hdr.element_size() * hdr.dimension_size(0);
    if (line_size % 4 == 0)
        glPixelStorei(GL_PACK_ALIGNMENT, 4);
    else if (line_size % 2 == 0)
        glPixelStorei(GL_PACK_ALIGNMENT, 2);
    else
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glGetTexImage(target, 0, format, type, &(data[0]));
    glPixelStorei(GL_PACK_ALIGNMENT, pa_bak);

    if (reverse_y)
        img_reverse_y(hdr.dimension_size(1), line_size, &(data[0]));

    try {
        hdr.write_to(f);
        hdr.write_data(f, &(data[0]));
    }
    catch (std::exception& e) {
        std::fclose(f);
        return false;
    }
    std::fclose(f);
    return true;
}

#endif
