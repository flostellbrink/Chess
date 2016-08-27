#include <GL/glew.h>

#include "glm/glm.hpp"
#include "revolver.hpp"
#include "gui/config.h"
#include <iostream>

using glm::cross;

Revolver::Revolver(Curve* baseCurve, Curve* heightCurve, float scale, bool clockTexture)
    : _baseCurve(baseCurve), _heightCurve(heightCurve), _scale(scale), _clockTexture(clockTexture) {

}

void Revolver::Create() {
    // TODO respect heights max x and bases max size
    int texWidth = _scale * 4 + 1; // Texture needs to seemlesly wrap around

    vector<vec2> basePoints = _baseCurve->InterpolatedPoints(Config::localResolution),
        heightPoints = _heightCurve->InterpolatedPoints(Config::localResolution2),
        baseTangent = _baseCurve->InterpolatedTangents(),
        heightTangent = _heightCurve->InterpolatedTangents();

    int baseRes = basePoints.size(),
        heightRes = heightPoints.size();

    for (int i = 0; i < heightRes; ++i) {
        vec2 height = heightPoints[i];
        for (int j = 0; j < baseRes; ++j) {
            vec2 base = basePoints[j];

            _geoPositions.push_back(_scale * vec3(height.x * base.x, height.y, height.x * base.y));

            if(_clockTexture)
                _geoTextureCoords.push_back(vec2((height.x * base.x) / -2.f + .5f, (height.x * base.y) / -2.f + .5f));
            else
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
                _geoIndices.push_back((i + 0) * baseRes + (j + 0));
                _geoIndices.push_back((i + 0) * baseRes + (j + 1));
                _geoIndices.push_back((i + 1) * baseRes + (j + 0));
                _geoIndices.push_back((i + 0) * baseRes + (j + 1));
                _geoIndices.push_back((i + 1) * baseRes + (j + 1));
                _geoIndices.push_back((i + 1) * baseRes + (j + 0));
            }
        }
    }
}
