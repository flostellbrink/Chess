/*
 * Copyright (C) 2013, 2014
 * Computer Graphics Group, University of Siegen
 * Written by Martin Lambers <martin.lambers@uni-siegen.de>
 * All rights reserved.
 */

#ifndef TEXLOAD_H
#define TEXLOAD_H

#include <string>

/* Read and write textures from and to PNG files.
 * Return success (true) or error (false).
 *
 * Only RGB(A) images with 8-bit components are supported.
 *
 * The 'target' parameter must be the texture target, e.g. GL_TEXTURE_2D for
 * the currently bound 2D texture. The load function will overwrite the relevant
 * properties of the texture (size, internal format, data).
 *
 * If 'reverse_y' is set, these functions will reverse the y axis of the image
 * (PNG has line 0 at the top, whereas in OpenGL y=0 is bottom, except for cube
 * maps).
 *
 * Note that PNG usually stores sRGB colors, not linear colors!
 */
bool load_png(GLenum target, const std::string& filename, bool reverse_y = true);
bool save_png(GLenum target, const std::string& filename, bool reverse_y = true);

#ifdef HAVE_GTA
/* Read and write textures from and to GTA files.
 * Return success (true) or error (false).
 *
 * All numbers of components (1, 2, 3, 4) and all types (ubyte, byte, ushort,
 * short, uint, int, float) are supported.
 *
 * The 'target' parameter must be the texture target, e.g. GL_TEXTURE_2D for
 * the currently bound 2D texture. The load function will overwrite the relevant
 * properties of the texture (size, internal format, data).
 *
 * If 'reverse_y' is set, these functions will reverse the y axis of the image
 * (GTA has line 0 at the top, whereas in OpenGL y=0 is bottom, except for cube
 * maps).
 *
 * TODO: currently only 2D textures are handled. GTAs can also store 1D and
 * 3D; add support for these when required.
 *
 * TODO: support SRGB internal formats? GTAs can store the color space type.
 */
bool load_gta(GLenum target, const std::string& filename, bool reverse_y = true);
bool save_gta(GLenum target, const std::string& filename, bool reverse_y = true);
#endif

#endif
