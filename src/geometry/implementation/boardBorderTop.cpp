#include <GL/glew.h>

#include "boardBorderTop.hpp"
#include <iostream>

using glm::vec3;

BoardBorderTop::BoardBorderTop()
{

}

void BoardBorderTop::Create() {
    // Positions
    _geoPositions.push_back(vec3(-8, 0, -8));
    _geoPositions.push_back(vec3(-8.5, 0, -8.5));
    _geoPositions.push_back(vec3(8, 0, -8));
    _geoPositions.push_back(vec3(8.5, 0, -8.5));

    // Normals
    _geoNormals.push_back(vec3(0, 1, 0));
    _geoNormals.push_back(vec3(0, 1, 0));
    _geoNormals.push_back(vec3(0, 1, 0));
    _geoNormals.push_back(vec3(0, 1, 0));

    // Texture Coordinates not needed, mapped from world position
	// How wrong i was.. without them this breaks on windows
	for (int i = 0; i < 4 * 1; ++i)
		_geoTextureCoords.push_back(vec2());

    // Indices
    _geoIndices.push_back(0);
    _geoIndices.push_back(1);
    _geoIndices.push_back(2);
    _geoIndices.push_back(1);
    _geoIndices.push_back(2);
    _geoIndices.push_back(3);
}
