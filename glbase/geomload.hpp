/*
 * Copyright (C) 2013, 2014
 * Computer Graphics Group, University of Siegen
 * Written by Martin Lambers <martin.lambers@uni-siegen.de>
 * All rights reserved.
 */

#ifndef GEOMLOAD_H
#define GEOMLOAD_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
namespace glm { typedef glm::detail::tvec3<unsigned char, glm::highp> ubvec3; }

/* These functions read/write geometry information from/to files.
 *
 * The interface is intentionally primitive, so that low-level access for OpenGL
 * is easy. Materials are not supported.
 *
 * When reading, the arrays will be cleared and filled with information from the
 * geometry file. The data is suitable for rendering with glDrawElements() in
 * GL_TRIANGLES mode. If a vertex attribute is not available (e.g. texcoords),
 * then the corresponding array will remain empty. If triangle information is not
 * available (e.g. with point clouds), then the index array will remain empty.
 *
 * When writing, the same conventions are expected from the input data.
 *
 * Note that vertex colors are not supported by the OBJ format.
 */

/* Read a file, autodetect the file type. */
bool load_geom(const std::string& filename,
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec2>& texcoords,
        std::vector<glm::ubvec3>& colors,
        std::vector<unsigned int>& indices);

/* Save a file, autodetect the file type. */
bool save_geom(const std::string& filename,
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec3>& normals,
        const std::vector<glm::vec2>& texcoords,
        const std::vector<glm::ubvec3>& colors,
        const std::vector<unsigned int>& indices);

/* Read a PLY file */
bool load_ply(const std::string& filename,
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec2>& texcoords,
        std::vector<glm::ubvec3>& colors,
        std::vector<unsigned int>& indices);

/* Write a PLY file */
bool save_ply(const std::string& filename,
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec3>& normals,
        const std::vector<glm::vec2>& texcoords,
        const std::vector<glm::ubvec3>& colors,
        const std::vector<unsigned int>& indices);

/* Read an OBJ file (or at least a simple subset of OBJ files) */
bool load_obj(const std::string& filename,
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec2>& texcoords,
        std::vector<glm::ubvec3>& colors,
        std::vector<unsigned int>& indices);

/* Write an OBJ file (the colors are currently ignored...) */
bool save_obj(const std::string& filename,
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec3>& normals,
        const std::vector<glm::vec2>& texcoords,
        const std::vector<glm::ubvec3>& colors,
        const std::vector<unsigned int>& indices);

#endif
