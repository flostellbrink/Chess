/*
 * Copyright (C) 2013, 2014
 * Computer Graphics Group, University of Siegen
 * Written by Martin Lambers <martin.lambers@uni-siegen.de>
 * All rights reserved.
 */

#include <string>
#include <vector>
#include <cstdio>
#include <cstddef>
#include <cstring>
#include <cctype>

#include <glm/glm.hpp>

#include "ply.h"
#include "tiny_obj_loader.h"

#include "geomload.hpp"

using namespace glm;


static std::string suffix(const std::string& filename)
{
    std::string suffix;
    size_t dot_pos = filename.find_last_of('.');
    if (dot_pos != std::string::npos) {
        suffix = filename.substr(dot_pos + 1);
        for (size_t i = 0; i < suffix.length(); i++)
            suffix[i] = std::tolower(suffix[i]);
    }
    return suffix;
}

bool load_geom(const std::string& filename,
        std::vector<vec3>& positions,
        std::vector<vec3>& normals,
        std::vector<vec2>& texcoords,
        std::vector<ubvec3>& colors,
        std::vector<unsigned int>& indices)
{
    if (suffix(filename) == "ply")
        return load_ply(filename, positions, normals, texcoords, colors, indices);
    else
        return load_obj(filename, positions, normals, texcoords, colors, indices);
}

bool save_geom(const std::string& filename,
        const std::vector<vec3>& positions,
        const std::vector<vec3>& normals,
        const std::vector<vec2>& texcoords,
        const std::vector<ubvec3>& colors,
        const std::vector<unsigned int>& indices)
{
    if (suffix(filename) == "ply")
        return save_ply(filename, positions, normals, texcoords, colors, indices);
    else
        return save_obj(filename, positions, normals, texcoords, colors, indices);
}

bool load_ply(const std::string& filename,
        std::vector<vec3>& positions,
        std::vector<vec3>& normals,
        std::vector<vec2>& texcoords,
        std::vector<ubvec3>& colors,
        std::vector<unsigned int>& indices)
{
    positions.clear();
    normals.clear();
    texcoords.clear();
    colors.clear();
    indices.clear();

    FILE* f = std::fopen(filename.c_str(), "rb");

    int nelems;
    char **elist;
    PlyFile* ply = ply_read(f, &nelems, &elist);
    if (!ply) {
        fprintf(stderr, "%s: cannot read file\n", filename.c_str());
        return false;
    }

    for (int i = 0; i < nelems; i++) {
        if (std::strcmp(elist[i], "vertex") == 0) {
            typedef struct {
                float x, y, z;
                float nx, ny, nz;
                float u, v;
                unsigned char r, g, b;
            } ply_vertex_t;
            ply_vertex_t ply_vertex = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 0 };
            int num_elems = 0;
            int nprops = 0;
            PlyProperty** plyprop = ply_get_element_description(
                    ply, const_cast<char *>("vertex"), &num_elems, &nprops);
            for (int i = 0; i < nprops; i++) {
                std::string propname = plyprop[i]->name;
                if (propname == "x") {
                    plyprop[i]->internal_type = PLY_FLOAT;
                    plyprop[i]->offset = offsetof(ply_vertex_t, x);
                    ply_vertex.x = 1.0f;
                    ply_get_property(ply, "vertex", plyprop[i]);
                } else if (propname == "y") {
                    plyprop[i]->internal_type = PLY_FLOAT;
                    plyprop[i]->offset = offsetof(ply_vertex_t, y);
                    ply_vertex.y = 1.0f;
                    ply_get_property(ply, "vertex", plyprop[i]);
                } else if (propname == "z") {
                    plyprop[i]->internal_type = PLY_FLOAT;
                    plyprop[i]->offset = offsetof(ply_vertex_t, z);
                    ply_vertex.z = 1.0f;
                    ply_get_property(ply, "vertex", plyprop[i]);
                } else if (propname == "nx" || propname == "normal_x") {
                    plyprop[i]->internal_type = PLY_FLOAT;
                    plyprop[i]->offset = offsetof(ply_vertex_t, nx);
                    ply_vertex.nx = 1.0f;
                    ply_get_property(ply, "vertex", plyprop[i]);
                } else if (propname == "ny" || propname == "normal_y") {
                    plyprop[i]->internal_type = PLY_FLOAT;
                    plyprop[i]->offset = offsetof(ply_vertex_t, ny);
                    ply_vertex.ny = 1.0f;
                    ply_get_property(ply, "vertex", plyprop[i]);
                } else if (propname == "nz" || propname == "normal_z") {
                    plyprop[i]->internal_type = PLY_FLOAT;
                    plyprop[i]->offset = offsetof(ply_vertex_t, nz);
                    ply_vertex.nz = 1.0f;
                    ply_get_property(ply, "vertex", plyprop[i]);
                } else if (propname == "s" || propname == "u") {
                    plyprop[i]->internal_type = PLY_FLOAT;
                    plyprop[i]->offset = offsetof(ply_vertex_t, u);
                    ply_vertex.u = 1.0f;
                    ply_get_property(ply, "vertex", plyprop[i]);
                } else if (propname == "t" || propname == "v") {
                    plyprop[i]->internal_type = PLY_FLOAT;
                    plyprop[i]->offset = offsetof(ply_vertex_t, v);
                    ply_vertex.v = 1.0f;
                    ply_get_property(ply, "vertex", plyprop[i]);
                } else if (propname == "r" || propname == "red" || propname == "diffuse_red") {
                    plyprop[i]->internal_type = PLY_UCHAR;
                    plyprop[i]->offset = offsetof(ply_vertex_t, r);
                    ply_vertex.r = 1;
                    ply_get_property(ply, "vertex", plyprop[i]);
                } else if (propname == "g" || propname == "green" || propname == "diffuse_green") {
                    plyprop[i]->internal_type = PLY_UCHAR;
                    plyprop[i]->offset = offsetof(ply_vertex_t, g);
                    ply_vertex.g = 1;
                    ply_get_property(ply, "vertex", plyprop[i]);
                } else if (propname == "b" || propname == "blue" || propname == "diffuse_blue") {
                    plyprop[i]->internal_type = PLY_UCHAR;
                    plyprop[i]->offset = offsetof(ply_vertex_t, b);
                    ply_vertex.b = 1;
                    ply_get_property(ply, "vertex", plyprop[i]);
                }
            }
            bool have_positions = ply_vertex.x && ply_vertex.y && ply_vertex.z;
            bool have_normals = ply_vertex.nx && ply_vertex.ny && ply_vertex.nz;
            bool have_texcoords = ply_vertex.u && ply_vertex.v;
            bool have_colors = ply_vertex.r && ply_vertex.g && ply_vertex.b;
            if (have_positions)
                positions.resize(num_elems);
            if (have_normals)
                normals.resize(num_elems);
            if (have_texcoords)
                texcoords.resize(num_elems);
            if (have_colors)
                colors.resize(num_elems);
            for (int i = 0; i < num_elems; i++) {
                ply_get_element(ply, &ply_vertex);
                if (have_positions)
                    positions[i] = vec3(ply_vertex.x, ply_vertex.y, ply_vertex.z);
                if (have_normals)
                    normals[i] = vec3(ply_vertex.nx, ply_vertex.ny, ply_vertex.nz);
                if (have_texcoords)
                    texcoords[i] = vec2(ply_vertex.u, ply_vertex.v);
                if (have_colors)
                    colors[i] = ubvec3(ply_vertex.r, ply_vertex.g, ply_vertex.b);
            }
        } else if (std::strcmp(elist[i], "face") == 0) {
            int num_elems = 0;
            int nprops = 0;
            PlyProperty** plyprop = ply_get_element_description(
                    ply, const_cast<char *>("face"), &num_elems, &nprops);
            typedef struct {
                unsigned char vertices;
                unsigned int* indices;
            } ply_face_t;
            ply_face_t ply_face = { 0, NULL };
            for (int i = 0; i < nprops; i++) {
                std::string propname = plyprop[i]->name;
                if (propname == "vertex_indices") {
                    plyprop[i]->internal_type = PLY_UINT;
                    plyprop[i]->offset = offsetof(ply_face_t, indices);
                    plyprop[i]->count_internal = PLY_UCHAR;
                    plyprop[i]->count_offset = offsetof(ply_face_t, vertices);
                    ply_face.vertices = 1;
                    ply_get_property(ply, "face", plyprop[i]);
                    break;
                }
            }
            if (ply_face.vertices) {
                std::vector<unsigned int> tmp_indices(3 * num_elems);
                for (int i = 0; i < num_elems; i++) {
                    ply_get_element(ply, &ply_face);
                    if (ply_face.vertices != 3) {
                        fprintf(stderr, "%s: cannot handle non-triangular faces\n", filename.c_str());
                        tmp_indices.clear();
                        break;
                    }
                    tmp_indices[3 * i + 0] = ply_face.indices[0];
                    tmp_indices[3 * i + 1] = ply_face.indices[1];
                    tmp_indices[3 * i + 2] = ply_face.indices[2];
                }
                indices = tmp_indices;
            }
        }
    }
    std::fclose(f);
    return true;
}

bool save_ply(const std::string& filename,
        const std::vector<vec3>& positions,
        const std::vector<vec3>& normals,
        const std::vector<vec2>& texcoords,
        const std::vector<ubvec3>& colors,
        const std::vector<unsigned int>& indices)
{
    union {
        int i;
        unsigned char c[sizeof(int)];
    } endianness_test;
    endianness_test.i = 1;

    FILE* f = std::fopen(filename.c_str(), "wb");
    const char* elem_names[] = { "vertex", "face" };
    PlyFile* ply = ply_write(f, indices.size() > 0 ? 2 : 1, elem_names,
            endianness_test.c[0] ? PLY_BINARY_LE : PLY_BINARY_BE);
    if (!ply) {
        fprintf(stderr, "%s: cannot write file\n", filename.c_str());
        return false;
    }
    ply_element_count(ply, "vertex", positions.size());

    PlyProperty prop = { "", PLY_FLOAT, PLY_FLOAT, 0, 0, 0, 0, 0 };

    prop.external_type = PLY_FLOAT;
    prop.internal_type = PLY_FLOAT;
    prop.name = "x";
    ply_describe_property(ply, "vertex", &prop);
    prop.offset += sizeof(float);
    prop.name = "y";
    ply_describe_property(ply, "vertex", &prop);
    prop.offset += sizeof(float);
    prop.name = "z";
    ply_describe_property(ply, "vertex", &prop);
    prop.offset += sizeof(float);

    if (normals.size() == positions.size()) {
        prop.external_type = PLY_FLOAT;
        prop.internal_type = PLY_FLOAT;
        prop.name = "nx";
        ply_describe_property(ply, "vertex", &prop);
        prop.offset += sizeof(float);
        prop.name = "ny";
        ply_describe_property(ply, "vertex", &prop);
        prop.offset += sizeof(float);
        prop.name = "nz";
        ply_describe_property(ply, "vertex", &prop);
        prop.offset += sizeof(float);
    }

    if (texcoords.size() == positions.size()) {
        prop.external_type = PLY_FLOAT;
        prop.internal_type = PLY_FLOAT;
        prop.name = "s";
        ply_describe_property(ply, "vertex", &prop);
        prop.offset += sizeof(float);
        prop.name = "t";
        ply_describe_property(ply, "vertex", &prop);
        prop.offset += sizeof(float);
    }

    if (colors.size() == positions.size()) {
        prop.external_type = PLY_UCHAR;
        prop.internal_type = PLY_UCHAR;
        prop.name = "red";
        ply_describe_property(ply, "vertex", &prop);
        prop.offset += sizeof(unsigned char);
        prop.name = "green";
        ply_describe_property(ply, "vertex", &prop);
        prop.offset += sizeof(unsigned char);
        prop.name = "blue";
        ply_describe_property(ply, "vertex", &prop);
        prop.offset += sizeof(unsigned char);
    }

    struct Face {
        unsigned char nverts;
        unsigned int *verts;
    } face;

    if (indices.size() > 0) {
        ply_element_count(ply, "face", indices.size() / 3);
        PlyProperty face_prop = {
            "vertex_indices", PLY_INT, PLY_UINT, offsetof(struct Face, verts),
            1, PLY_UCHAR, PLY_UCHAR, offsetof(struct Face, nverts)
        };
        ply_describe_property(ply, "face", &face_prop);
    }

    ply_header_complete(ply);

    ply_put_element_setup(ply, "vertex");
    std::vector<unsigned char> element(prop.offset);
    for (size_t i = 0; i < positions.size(); i++) {
        size_t offset = 0;
        memcpy(&(element[offset]), &positions[i], sizeof(vec3));
        offset += sizeof(vec3);
        if (normals.size() == positions.size()) {
            memcpy(&(element[offset]), &normals[i], sizeof(vec3));
            offset += sizeof(vec3);
        }
        if (texcoords.size() == positions.size()) {
            memcpy(&(element[offset]), &texcoords[i], sizeof(vec2));
            offset += sizeof(vec2);
        }
        if (colors.size() == positions.size()) {
            memcpy(&(element[offset]), &colors[i], sizeof(ubvec3));
            offset += sizeof(ubvec3);
        }
        ply_put_element(ply, static_cast<void*>(&element[0]));
    }

    if (indices.size() > 0) {
        ply_put_element_setup(ply, "face");
        face.nverts = 3;
        for (size_t i = 0; i < indices.size(); i += 3) {
            face.verts = const_cast<unsigned int*>(&indices[i]);
            ply_put_element(ply, &face);
        }
    }

    if (fclose(f) != 0) {
        fprintf(stderr, "%s: output error\n", filename.c_str());
        return false;
    }

    //ply_close(ply); this crashes
    return true;
}

bool load_obj(const std::string& filename,
        std::vector<vec3>& positions,
        std::vector<vec3>& normals,
        std::vector<vec2>& texcoords,
        std::vector<ubvec3>& colors,
        std::vector<unsigned int>& indices)
{
    positions.clear();
    normals.clear();
    texcoords.clear();
    colors.clear();
    indices.clear();

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string errmsg = tinyobj::LoadObj(shapes, materials, filename.c_str());
    if (!errmsg.empty()) {
        fprintf(stderr, "%s\n", errmsg.c_str());
        return false;
    }
    if (shapes.size() == 0) {
        fprintf(stderr, "%s: cannot understand data\n", filename.c_str());
        return false;
    }

    bool have_normals = (shapes[0].mesh.normals.size() / 3 == shapes[0].mesh.positions.size() / 3);
    bool have_texcoords = (shapes[0].mesh.texcoords.size() / 2 == shapes[0].mesh.positions.size() / 3);
    size_t start_index = 0;
    for (size_t i = 0; i < shapes.size(); i++) {
        start_index = positions.size();
        if (shapes[i].mesh.positions.size() == 0
                || shapes[i].mesh.positions.size() % 3 != 0
                || shapes[i].mesh.normals.size() % 3 != 0
                || (have_normals && shapes[i].mesh.normals.size() / 3 != shapes[i].mesh.positions.size() / 3)
                || shapes[i].mesh.texcoords.size() % 2 != 0
                || (have_texcoords && shapes[i].mesh.texcoords.size() / 2 != shapes[i].mesh.positions.size() / 3)
                || shapes[i].mesh.indices.size() == 0
                || shapes[i].mesh.indices.size() % 3 != 0) {
            positions.clear();
            normals.clear();
            texcoords.clear();
            indices.clear();
            fprintf(stderr, "%s: cannot understand data\n", filename.c_str());
            return false;
        }
        for (size_t j = 0; j < shapes[i].mesh.positions.size() / 3; j++) {
            positions.push_back(vec3(
                        shapes[i].mesh.positions[3 * j + 0],
                        shapes[i].mesh.positions[3 * j + 1],
                        shapes[i].mesh.positions[3 * j + 2]));
            if (have_normals) {
                normals.push_back(vec3(
                            shapes[i].mesh.normals[3 * j + 0],
                            shapes[i].mesh.normals[3 * j + 1],
                            shapes[i].mesh.normals[3 * j + 2]));
            }
            if (have_texcoords) {
                texcoords.push_back(vec2(
                            shapes[i].mesh.texcoords[2 * j + 0],
                            shapes[i].mesh.texcoords[2 * j + 1]));
            }
        }
        for (size_t j = 0; j < shapes[i].mesh.indices.size(); j++) {
            indices.push_back(start_index + shapes[i].mesh.indices[j]);
        }
    }

    return true;
}

bool save_obj(const std::string& filename,
        const std::vector<vec3>& positions,
        const std::vector<vec3>& normals,
        const std::vector<vec2>& texcoords,
        const std::vector<ubvec3>& /* colors */,
        const std::vector<unsigned int>& indices)
{
    FILE* f = std::fopen(filename.c_str(), "wb");
    if (!f) {
        fprintf(stderr, "%s: cannot write file\n", filename.c_str());
        return false;
    }
    fprintf(f, "# This is a Wavefront .obj file\n");

    for (size_t i = 0; i < positions.size(); i++) {
        fprintf(f, "v %.8g %.8g %.8g\n", positions[i].x, positions[i].y, positions[i].z);
    }
    if (normals.size() == positions.size()) {
        for (size_t i = 0; i < positions.size(); i++) {
            fprintf(f, "vn %.8g %.8g %.8g\n", normals[i].x, normals[i].y, normals[i].z);
        }
    }
    if (texcoords.size() == positions.size()) {
        for (size_t i = 0; i < positions.size(); i++) {
            fprintf(f, "vt %.8g %.8g\n", texcoords[i].s, texcoords[i].t);
        }
    }

    for (size_t i = 0; i < indices.size(); i += 3) {
        int ind[3];
        for (int j = 0; j < 3; j++)
            ind[j] = indices[i + j] + 1;
        if (normals.size() != positions.size() && texcoords.size() != positions.size()) {
            fprintf(f, "f %u %u %u\n",
                    ind[0], ind[1], ind[2]);
        } else if (normals.size() == positions.size() && texcoords.size() == positions.size()) {
            fprintf(f, "f %u/%u/%u %u/%u/%u %u/%u/%u\n",
                    ind[0], ind[0], ind[0],
                    ind[1], ind[1], ind[1],
                    ind[2], ind[2], ind[2]);
        } else if (normals.size() == positions.size()) {
            fprintf(f, "f %u//%u %u//%u %u//%u\n",
                    ind[0], ind[0],
                    ind[1], ind[1],
                    ind[2], ind[2]);
        } else if (texcoords.size() == positions.size()) {
            fprintf(f, "f %u/%u %u/%u %u/%u\n",
                    ind[0], ind[0],
                    ind[1], ind[1],
                    ind[2], ind[2]);
        }
    }

    if (fclose(f) != 0) {
        fprintf(stderr, "%s: output error\n", filename.c_str());
        return false;
    }

    return true;
}
