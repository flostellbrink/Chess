#include <GL/glew.h>
#include <glm/common.hpp>
#include <iostream>

#include <glm/gtx/transform.hpp>

#include "math.h"

#include "circleCurve.hpp"

using std::cos;
using std::sin;

CircleCurve::CircleCurve(float radius) : _radius(radius)
{

}

vector<vec2> CircleCurve::InterpolatedPoints(int resolution){
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

vec2 CircleCurve::InterpolatedPoint(float value){
    value *= glm::pi<float>() * 2.f;
    return _radius * vec2(cos(value), sin(value));
}

vector<vec2> CircleCurve::InterpolatedTangents(){
    return _tangents;
}

vec2 CircleCurve::InterpolatedTangent(float value){
    value -= .25;
    value *= glm::pi<float>() * 2.f;
    return vec2(cos(value), sin(value));
}