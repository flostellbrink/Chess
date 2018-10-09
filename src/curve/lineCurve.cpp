#include <GL/glew.h>
#include <glm/common.hpp>
#include <iostream>

#include <glm/gtx/transform.hpp>

#include "math.h"

#include "lineCurve.hpp"

LineCurve::LineCurve(float radius) : _radius(radius) {

}

vector<vec2> LineCurve::InterpolatedPoints(int resolution) {
    float stepSize = 1.f / (resolution -1);
    float value = 0;
    vector<vec2> result;
    _tangents.clear();
    for (int i = 0; i < resolution; ++i) {
        value = i * stepSize;
        result.push_back(InterpolatedPoint(value));
        _tangents.push_back(InterpolatedTangent(value));
    }
    return result;
}

vec2 LineCurve::InterpolatedPoint(float value){
    // Move value into -1 1 range
    value = (value - .5f) * 2;
    return vec2(value * 2, cos(value * glm::pi<float>() * _radius));
}

vector<vec2> LineCurve::InterpolatedTangents(){
    return _tangents;
}

vec2 LineCurve::InterpolatedTangent(float value){
    // Move value into -1 1 range
    value = (value - .5f) * 2;
    value *= glm::pi<float>() * _radius;
    // Shift by a quarter rotation
    value -= glm::pi<float>() * .25f;
    return vec2(sin(value), cos(value));
}
