#include <GL/glew.h>

#include "glm/glm.hpp"
#include "extruder.hpp"
#include <iostream>

#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm/gtx/transform.hpp>
#include "gui/config.h"

#include "math.h"
#ifndef M_PI
#define M_PI glm::pi<float>()
#endif

using glm::cross;

Extruder::Extruder(Curve* profileCurve, Curve* widthCurve, float scale) : _scale(scale){
    _profileCurve = profileCurve;
    _widthCurve = widthCurve;
}

void Extruder::Create() {
    vector<vec2> profilePoints = _profileCurve->InterpolatedPoints(Config::geoResolution2),
            profileTangents = _profileCurve->InterpolatedTangents(),
            widthPoints = _widthCurve->InterpolatedPoints(_resolution),
            widthTangents = _widthCurve->InterpolatedTangents();
    int profileRes = profilePoints.size(),
            widthRes = widthPoints.size();
    for (int i = 0; i < widthRes; ++i) {
        vec2 width = widthPoints[i];
        for (int j = 0; j < profileRes; ++j) {
            vec2 profile = profilePoints[j];
            _geoPositions.push_back(_scale * vec3(profile.x * width.y,
                                         profile.y,
                                         width.x));
            _geoTextureCoords.push_back(vec2(width.x * _scale,
                                             j == 0 ? 0 : glm::length(profilePoints[j-1] - profile) * _scale));
            _geoNormals.push_back(glm::normalize(cross(vec3(profileTangents[j].x, profileTangents[j].y, 0),
                                        vec3(0, 0, widthTangents[i].x))));
            if(i < widthRes - 1 && j < profileRes - 1){
                _geoIndices.push_back((i + 0) * profileRes + (j + 0));
                _geoIndices.push_back((i + 0) * profileRes + (j + 1));
                _geoIndices.push_back((i + 1) * profileRes + (j + 0));
                _geoIndices.push_back((i + 0) * profileRes + (j + 1));
                _geoIndices.push_back((i + 1) * profileRes + (j + 1));
                _geoIndices.push_back((i + 1) * profileRes + (j + 0));
            }
        }
    }
}
