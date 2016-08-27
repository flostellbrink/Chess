/*
 * Copyright (C) 2013
 * Computer Graphics Group, University of Siegen
 * Written by Martin Lambers <martin.lambers@uni-siegen.de>
 * All rights reserved.
 */

#include <vector>

#include <glm/glm.hpp>
namespace glm { typedef glm::detail::tvec3<unsigned char, glm::highp> ubvec3; }


#ifndef GEOMETRIES_H
#define GEOMETRIES_H

/* These functions return basic geometries, scaled to fill [-1,+1]^3.
 * The arrays are cleared and geometry data is written to them. This
 * data is suitable for rendering with glDrawElements() in GL_TRIANGLES mode.
 *
 * These are replacements for glutSolid*()/glutWired*() etc. */

void geom_quad(
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec2>& texcoords,
        std::vector<unsigned int>& indices);

void geom_cube(
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec2>& texcoords,
        std::vector<unsigned int>& indices);

void geom_disk(
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec2>& texcoords,
        std::vector<unsigned int>& indices,
        float inner_radius = 0.2f, int slices = 40);

void geom_sphere(
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec2>& texcoords,
        std::vector<unsigned int>& indices,
        int slices = 40, int stacks = 20);

void geom_cylinder(
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec2>& texcoords,
        std::vector<unsigned int>& indices,
        int slices = 40);

void geom_cone(
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec2>& texcoords,
        std::vector<unsigned int>& indices,
        int slices = 40, int stacks = 20);

void geom_torus(
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec2>& texcoords,
        std::vector<unsigned int>& indices,
        float inner_radius = 0.4f, int sides = 40, int rings = 40);

void geom_teapot(
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec2>& texcoords,
        std::vector<unsigned int>& indices);

/* For a given geometry in GL_TRIANGLES mode with indices, create a new index list
 * that provides GL_TRIANGLES_ADJACENCY. This is useful for geometry shaders.
 * If a neighboring triangle is not found for an edge of a given triangle, the
 * neighbor for that edge will be set to the triangle itself, only in opposite direction.
 * WARNING: the current implementation is O(n²). Only use once on initialization, and
 * don't use with larger models. */
std::vector<unsigned int> create_adjacency(const std::vector<unsigned int>& indices);

#endif
