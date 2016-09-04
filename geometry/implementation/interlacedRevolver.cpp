#include <GL/glew.h>

#include "glm/glm.hpp"
#include "interlacedRevolver.hpp"
#include <iostream>
#include "gui/config.h"

using glm::cross;

InterlacedRevolver::InterlacedRevolver(Curve* baseCurve, Curve* heightCurve1, Curve* heightCurve2, float length1, float length2, float scale)
    :_scale(scale), _baseCurve(baseCurve), _heightCurve1(heightCurve1), _heightCurve2(heightCurve2), _length1(length1), _length2(length2)  {

}

void InterlacedRevolver::Create() {
    // TODO respect curves max values. see revolver
    int texWidth = _scale * 4 + 1; // Texture needs to seemlesly wrap around

    vector<vec2> basePoints = _baseCurve->InterpolatedPoints(Config::geoResolution1),
        baseTangent = _baseCurve->InterpolatedTangents();
    int baseRes = basePoints.size();

    for (int j = 0; j < baseRes; ++j) {
        vec2 base = basePoints[j];

        // value keeps track of the rotation to switch between curves
        float value = (float)j / (float)baseRes + _length1 / .2; // Offset to avoid straight edges
        while(value > _length1 + _length2)
            value -= _length1 + _length2;

        Curve* heightCurve = value < _length1 ? _heightCurve1 : _heightCurve2;

        vector<vec2> heightPoints = heightCurve->InterpolatedPoints(Config::geoResolution2),
                heightTangent = heightCurve->InterpolatedTangents();
        int heightRes = heightPoints.size();

        for (int i = 0; i < heightRes; ++i) {
            vec2 height = heightPoints[i];

            _geoPositions.push_back(_scale * vec3(height.x * base.x, height.y, height.x * base.y));

            _geoTextureCoords.push_back(vec2((float)j / ((float)baseRes - 1) * (float)texWidth,
                                             i == 0 ? 0 : glm::length(heightPoints[i-1] - height) * _scale / 5));

            if(i < heightRes - 1)
                _geoNormals.push_back(glm::normalize(cross(
                    vec3(base.x * heightTangent[i].x, heightTangent[i].y, base.y * heightTangent[i].x),
                    vec3(baseTangent[j].x, 0, baseTangent[j].y))));
            else
                // Last points tangent is ususally undefined. set it to up
                _geoNormals.push_back(vec3(0,1,0));

            if(i < heightRes - 1 && j < baseRes - 1){
                _geoIndices.push_back((i + 0) + heightRes * (j + 0));
                _geoIndices.push_back((i + 0) + heightRes * (j + 1));
                _geoIndices.push_back((i + 1) + heightRes * (j + 0));
                _geoIndices.push_back((i + 0) + heightRes * (j + 1));
                _geoIndices.push_back((i + 1) + heightRes * (j + 1));
                _geoIndices.push_back((i + 1) + heightRes * (j + 0));
            }
        }
    }
}
