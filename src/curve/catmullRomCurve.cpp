#include <GL/glew.h>
#include <iostream>

#include "math.h"
#ifndef M_PI
#define M_PI glm::pi<float>()
#endif

#include "catmullRomCurve.hpp"

CatmullRomCurve::CatmullRomCurve(ControlPoints controlPoints, int curveID, int objectID){
    _controlPoints = controlPoints;
    _curveID = curveID;
    _objectID = objectID;
}

vector<vec2> CatmullRomCurve::InterpolatedPoints(int resolution){
    _lastResolution = resolution;
    vector<vec2> result;
    float stepSize = 1.0f / (float)(resolution - 1);
    for (int i = 0; i < resolution; ++i)
        result.push_back(InterpolatedPoint(i * stepSize));
    return result;
}

vec2 CatmullRomCurve::InterpolatedPoint(float value){
    vector<vec2> controlPoints = _controlPoints.GetControlPoints2D(_objectID)[_curveID];
    // Figure out what segment we are in
    int segments = controlPoints.size() / 3 - 1;
    value *= segments;
    int segment = value;
    if (segment == segments)
        segment--;
    value -= segment;

    int startIndex = segment * 3 + 1;

    vec2 point1 = controlPoints.at(startIndex), point2 = controlPoints.at(startIndex + 1),
         point3 = controlPoints.at(startIndex + 2), point4 = controlPoints.at(startIndex + 3);

    // Calculate the result using de Casteljau
    return  point1 * (float) pow(1 - value, 3)
        + point2 * (float) (3 * pow(1 - value, 2) * value)
        + point3 * (float) (3 * (1 - value) * pow(value, 2))
        + point4 * (float) pow(value, 3);
}

vector<vec2> CatmullRomCurve::InterpolatedTangents(){
    vector<vec2> result;
    float stepSize = 1.0f / (float)(_lastResolution - 1);
    for (int i = 0; i < _lastResolution; ++i)
        result.push_back(InterpolatedTangent(i * stepSize));
    return result;
}

vec2 CatmullRomCurve::InterpolatedTangent(float value){
    vector<vec2> controlPoints = _controlPoints.GetControlPoints2D(_objectID)[_curveID];
    // Figure out what segment we are in
    int segments = controlPoints.size() / 3 - 1;
    value *= segments;
    int segment = value;
    if (segment == segments)
        segment--;
    value -= segment;

    int startIndex = segment * 3 + 1;

    vec2 point1 = controlPoints.at(startIndex), point2 = controlPoints.at(startIndex + 1),
         point3 = controlPoints.at(startIndex + 2), point4 = controlPoints.at(startIndex + 3);

    // Calculate the result using first derivative of de Casteljau
    return  3.f * ((point4 - 3.f * point3 + 3.f * point2 - point1) * (float)pow(value, 2)
        + (2.f * point3 - 4.f *point2 + 2.f*point1) * value + point2 - point1);
}
