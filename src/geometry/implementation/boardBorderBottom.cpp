#include <GL/glew.h>

#include "boardBorderBottom.hpp"
#include <iostream>

using glm::vec3;

BoardBorderBottom::BoardBorderBottom()
{

}

void BoardBorderBottom::Create() {
    // Positions
    _geoPositions.push_back(vec3(-8.5, 0, -8.5));
    _geoPositions.push_back(vec3(-8.5, -.1, -8.5));
    _geoPositions.push_back(vec3(8.5, 0, -8.5));
    _geoPositions.push_back(vec3(8.5, -.1, -8.5));

    _geoPositions.push_back(vec3(-8.5, -.1, -8.5));
    _geoPositions.push_back(vec3(-10, -.8, -10));
    _geoPositions.push_back(vec3(8.5, -.1, -8.5));
    _geoPositions.push_back(vec3(10, -.8, -10));

    _geoPositions.push_back(vec3(-10, -.8, -10));
    _geoPositions.push_back(vec3(-10, -1, -10));
    _geoPositions.push_back(vec3(10, -.8, -10));
    _geoPositions.push_back(vec3(10, -1, -10));

    // Normals
    _geoNormals.push_back(vec3(0, 0, 1));
    _geoNormals.push_back(vec3(0, 0, 1));
    _geoNormals.push_back(vec3(0, 0, 1));
    _geoNormals.push_back(vec3(0, 0, 1));
    // TODO should be normalized?
    _geoNormals.push_back(vec3(0, 1, 1));
    _geoNormals.push_back(vec3(0, 1, 1));
    _geoNormals.push_back(vec3(0, 1, 1));
    _geoNormals.push_back(vec3(0, 1, 1));

    _geoNormals.push_back(vec3(0, 0, 1));
    _geoNormals.push_back(vec3(0, 0, 1));
    _geoNormals.push_back(vec3(0, 0, 1));
    _geoNormals.push_back(vec3(0, 0, 1));

	// Texture Coordinates, If these are not defined windows 7 breaks down.
	for (int i = 0; i < 4 * 3; ++i)
		_geoTextureCoords.push_back(vec2());

    // Texture Coordinates not needed, mapped from world position

    // Indices
    _geoIndices.push_back(0);
    _geoIndices.push_back(1);
    _geoIndices.push_back(2);
    _geoIndices.push_back(1);
    _geoIndices.push_back(2);
    _geoIndices.push_back(3);

    _geoIndices.push_back(4);
    _geoIndices.push_back(5);
    _geoIndices.push_back(6);
    _geoIndices.push_back(5);
    _geoIndices.push_back(6);
    _geoIndices.push_back(7);

    _geoIndices.push_back(8);
    _geoIndices.push_back(9);
    _geoIndices.push_back(10);
    _geoIndices.push_back(9);
    _geoIndices.push_back(10);
    _geoIndices.push_back(11);
}
