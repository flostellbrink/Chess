#include <GL/glew.h>

#include "cube.hpp"
#include <iostream>

using glm::vec3;

Cube::Cube()
{

}

void Cube::Create() {
    // Positions
    //front
    _geoPositions.push_back(vec3(-1, +1, +1));
    _geoPositions.push_back(vec3(+1, +1, +1));
    _geoPositions.push_back(vec3(+1, -1, +1));
    _geoPositions.push_back(vec3(-1, -1, +1));
    //back
    _geoPositions.push_back(vec3(-1, +1, -1));
    _geoPositions.push_back(vec3(+1, +1, -1));
    _geoPositions.push_back(vec3(+1, -1, -1));
    _geoPositions.push_back(vec3(-1, -1, -1));
    //right
    _geoPositions.push_back(vec3(+1, +1, -1));
    _geoPositions.push_back(vec3(+1, +1, +1));
    _geoPositions.push_back(vec3(+1, -1, +1));
    _geoPositions.push_back(vec3(+1, -1, -1));
    //left
    _geoPositions.push_back(vec3(-1, +1, -1));
    _geoPositions.push_back(vec3(-1, +1, +1));
    _geoPositions.push_back(vec3(-1, -1, +1));
    _geoPositions.push_back(vec3(-1, -1, -1));
    //top
    _geoPositions.push_back(vec3(-1, +1, +1));
    _geoPositions.push_back(vec3(+1, +1, +1));
    _geoPositions.push_back(vec3(+1, +1, -1));
    _geoPositions.push_back(vec3(-1, +1, -1));
    //bottom
    _geoPositions.push_back(vec3(-1, -1, +1));
    _geoPositions.push_back(vec3(+1, -1, +1));
    _geoPositions.push_back(vec3(+1, -1, -1));
    _geoPositions.push_back(vec3(-1, -1, -1));

    // Normals
    //front
    _geoNormals.push_back(vec3(+0, +0, +1));
    _geoNormals.push_back(vec3(+0, +0, +1));
    _geoNormals.push_back(vec3(+0, +0, +1));
    _geoNormals.push_back(vec3(+0, +0, +1));
    //back
    _geoNormals.push_back(vec3(+0, +0, -1));
    _geoNormals.push_back(vec3(+0, +0, -1));
    _geoNormals.push_back(vec3(+0, +0, -1));
    _geoNormals.push_back(vec3(+0, +0, -1));
    //right
    _geoNormals.push_back(vec3(+1, +0, +0));
    _geoNormals.push_back(vec3(+1, +0, +0));
    _geoNormals.push_back(vec3(+1, +0, +0));
    _geoNormals.push_back(vec3(+1, +0, +0));
    //left
    _geoNormals.push_back(vec3(-1, +0, +0));
    _geoNormals.push_back(vec3(-1, +0, +0));
    _geoNormals.push_back(vec3(-1, +0, +0));
    _geoNormals.push_back(vec3(-1, +0, +0));
    //top
    _geoNormals.push_back(vec3(+0, +1, +0));
    _geoNormals.push_back(vec3(+0, +1, +0));
    _geoNormals.push_back(vec3(+0, +1, +0));
    _geoNormals.push_back(vec3(+0, +1, +0));
    //bottom
    _geoNormals.push_back(vec3(+0, -1, +0));
    _geoNormals.push_back(vec3(+0, -1, +0));
    _geoNormals.push_back(vec3(+0, -1, +0));
    _geoNormals.push_back(vec3(+0, -1, +0));

    // Texture Coordinates
    // Todo, possibly have a second mode for a t shape cube texture
    //front
    _geoTextureCoords.push_back(vec2(0,1));
    _geoTextureCoords.push_back(vec2(1,1));
    _geoTextureCoords.push_back(vec2(1,0));
    _geoTextureCoords.push_back(vec2(0,0));
    //back
    _geoTextureCoords.push_back(vec2(1,1));
    _geoTextureCoords.push_back(vec2(0,1));
    _geoTextureCoords.push_back(vec2(0,0));
    _geoTextureCoords.push_back(vec2(1,0));
    //right
    _geoTextureCoords.push_back(vec2(1,1));
    _geoTextureCoords.push_back(vec2(0,1));
    _geoTextureCoords.push_back(vec2(0,0));
    _geoTextureCoords.push_back(vec2(1,0));
    //left
    _geoTextureCoords.push_back(vec2(0,1));
    _geoTextureCoords.push_back(vec2(1,1));
    _geoTextureCoords.push_back(vec2(1,0));
    _geoTextureCoords.push_back(vec2(0,0));
    //top
    _geoTextureCoords.push_back(vec2(0,0));
    _geoTextureCoords.push_back(vec2(1,0));
    _geoTextureCoords.push_back(vec2(1,1));
    _geoTextureCoords.push_back(vec2(0,1));
    //bottom
    _geoTextureCoords.push_back(vec2(0,1));
    _geoTextureCoords.push_back(vec2(1,1));
    _geoTextureCoords.push_back(vec2(1,0));
    _geoTextureCoords.push_back(vec2(0,0));

    // Indices
    //front
    _geoIndices.push_back(0);
    _geoIndices.push_back(1);
    _geoIndices.push_back(3);
    _geoIndices.push_back(1);
    _geoIndices.push_back(2);
    _geoIndices.push_back(3);
    //back
    _geoIndices.push_back(4);
    _geoIndices.push_back(5);
    _geoIndices.push_back(7);
    _geoIndices.push_back(5);
    _geoIndices.push_back(6);
    _geoIndices.push_back(7);
    //right
    _geoIndices.push_back(8);
    _geoIndices.push_back(9);
    _geoIndices.push_back(11);
    _geoIndices.push_back(9);
    _geoIndices.push_back(10);
    _geoIndices.push_back(11);
    //left
    _geoIndices.push_back(12);
    _geoIndices.push_back(13);
    _geoIndices.push_back(15);
    _geoIndices.push_back(13);
    _geoIndices.push_back(14);
    _geoIndices.push_back(15);
    //top
    _geoIndices.push_back(16);
    _geoIndices.push_back(17);
    _geoIndices.push_back(19);
    _geoIndices.push_back(17);
    _geoIndices.push_back(18);
    _geoIndices.push_back(19);
    //bottom
    _geoIndices.push_back(20);
    _geoIndices.push_back(21);
    _geoIndices.push_back(23);
    _geoIndices.push_back(21);
    _geoIndices.push_back(22);
    _geoIndices.push_back(23);
}
