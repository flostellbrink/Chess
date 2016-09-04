#include <GL/glew.h>

#include "glm/glm.hpp"
#include "extruderFace.hpp"
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

ExtruderFace::ExtruderFace(Curve* profileCurve, Curve* compressedProfile, Curve* widthCurve, float widthPos, float scale) : _scale(scale){
    _profileCurve = profileCurve;
    _compressedProfile = compressedProfile;
    _widthCurve = widthCurve;
    _widthPos = widthPos;
}

void ExtruderFace::Create() {
    float texScale = .1;
    vector<vec2> profilePoints = _profileCurve->InterpolatedPoints(Config::geoResolution2),
        compressedPoints = _compressedProfile->InterpolatedPoints(Config::geoResolution2);

    vec2 width = _widthCurve->InterpolatedPoint(_widthPos);
    vec3 widthTangent = glm::normalize(vec3(0, 0, _widthPos - .5));

    int profileRes = profilePoints.size(),
        compressedRes = compressedPoints.size();
    assert(profileRes == compressedRes);

    for (int j = 0; j < profileRes; ++j) {
        vec2 profile = profilePoints[j];
        _geoPositions.push_back(_scale * vec3(profile.x * width.y, profile.y, width.x));
        _geoTextureCoords.push_back(texScale * _scale * profile);
        _geoNormals.push_back(widthTangent);
    }

    for (int j = 0; j < compressedRes; ++j) {
        vec2 profile = compressedPoints[j];
        _geoPositions.push_back(_scale * vec3(profile.x * width.y, profile.y, width.x));
        _geoTextureCoords.push_back(texScale * _scale * profile);
        _geoNormals.push_back(widthTangent);

        _geoIndices.push_back(j + 0);
        _geoIndices.push_back(j + 1);
        _geoIndices.push_back(j + 0 + profileRes);
        _geoIndices.push_back(j + 1);
        _geoIndices.push_back(j + 1 + profileRes);
        _geoIndices.push_back(j + 0 + profileRes);
    }
}
