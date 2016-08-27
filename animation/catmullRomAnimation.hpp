#ifndef CATMULLROMANIMATION_H
#define CATMULLROMANIMATION_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm/gtx/transform.hpp>

#include "math.h"
#ifndef M_PI
#define M_PI glm::pi<float>()
#endif

#include <GL/gl.h>
#include "animation.hpp"
#include <vector>
#include <iostream>

using std::vector;

template<class T>
class CatmullRomAnimation : public Animation<T>{
    public:
    CatmullRomAnimation(int duration, T& property, vector<T> values);
    virtual T ValueAt(float value) override;

    protected:
    vector<T> _values;

    private:
};

// Same as in animation.hpp
template<class T>
CatmullRomAnimation<T>::CatmullRomAnimation(int duration, T& property, vector<T> values)
    : Animation<T>(duration, property){
    float tanFac = .166;
    if(values.size() > 1){
        T tangent = (values[1] - values[0]) * tanFac;
        _values.push_back(values[0]);
        _values.push_back(values[0]);
        _values.push_back(values[0] + tangent);
    }
    for(int i = 1; i < values.size() - 1; ++i){
        T tangent = (values[i+1] - values[i-1]) * tanFac;
        _values.push_back(values[i] - tangent);
        _values.push_back(values[i]);
        _values.push_back(values[i] + tangent);
    }
    if(values.size() > 1){
    T tangent = (values[1] - values[0]) * tanFac;
        _values.push_back(values[values.size() - 1] - tangent);
        _values.push_back(values[values.size() - 1]);
        _values.push_back(values[values.size() - 1]);
    }
}

template<class T>
T CatmullRomAnimation<T>::ValueAt(float value){
    // Figure out what segment we are in
    int segments = _values.size() / 3 - 1;
    value *= segments;
    int segment = value;
    if (segment == segments)
        segment--;
    value -= segment;

    int startIndex = segment * 3 + 1;

    T point1 = _values.at(startIndex), point2 = _values.at(startIndex + 1),
         point3 = _values.at(startIndex + 2), point4 = _values.at(startIndex + 3);

    // Calculate the result using de Casteljau
    return  point1 * (float) pow(1 - value, 3)
        + point2 * (float) (3 * pow(1 - value, 2) * value)
        + point3 * (float) (3 * (1 - value) * pow(value, 2))
        + point4 * (float) pow(value, 3);
}

#endif // CATMULLROMANIMATION_H
