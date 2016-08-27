#include <GL/glew.h>

#include "square.hpp"

Square::Square()
{

}

void Square::Create() {
    _geoPositions.push_back(vec3(-1,0,-1));
    _geoPositions.push_back(vec3(1,0,-1));
    _geoPositions.push_back(vec3(-1,0,1));
    _geoPositions.push_back(vec3(1,0,1));

    _geoIndices.push_back(1);
    _geoIndices.push_back(2);
    _geoIndices.push_back(3);

    _geoIndices.push_back(0);
    _geoIndices.push_back(2);
    _geoIndices.push_back(1);
}
