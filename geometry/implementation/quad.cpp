#include <GL/glew.h>

#include "quad.hpp"
#include <iostream>

using glm::vec3;

Quad::Quad()
{

}

void Quad::Create() {
    // Positions
    _geoPositions.push_back(vec3(+1, +1, +0));
    _geoPositions.push_back(vec3(+1, -1, +0));
    _geoPositions.push_back(vec3(-1, -1, +0));
    _geoPositions.push_back(vec3(-1, +1, +0));

    // Normals
    _geoNormals.push_back(vec3(+0, +0, +1));
    _geoNormals.push_back(vec3(+0, +0, +1));
    _geoNormals.push_back(vec3(+0, +0, +1));
    _geoNormals.push_back(vec3(+0, +0, +1));

    // Texture Coordinates
    _geoTextureCoords.push_back(vec2(1,1));
    _geoTextureCoords.push_back(vec2(1,0));
    _geoTextureCoords.push_back(vec2(0,0));
    _geoTextureCoords.push_back(vec2(0,1));

    // Indices
    _geoIndices.push_back(0);
    _geoIndices.push_back(1);
    _geoIndices.push_back(3);
    _geoIndices.push_back(1);
    _geoIndices.push_back(2);
    _geoIndices.push_back(3);
}
