#ifndef FADEANIMATION_H
#define FADEANIMATION_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#define GLM_SWIZZLE
#include <glm/gtx/transform.hpp>

#include "math.h"
#ifndef M_PI
#define M_PI glm::pi<float>()
#endif

#include <GL/gl.h>
#include "animation.hpp"

template<class T>
class FadeAnimation : public Animation<T>{
    public:
    FadeAnimation(int duration, T& property, T from, T to);
    virtual T ValueAt(float value) override;

    protected:
    T _from, _to;

    private:
};

//Same as in animation.hpp
template<class T>
FadeAnimation<T>::FadeAnimation(int duration, T& property, T from, T to)
    : Animation<T>(duration, property){
    _from = from;
    _to = to;
}

template<class T>
T FadeAnimation<T>::ValueAt(float value){
    auto easeValue = (float)(.5 * sin(value * M_PI - .5 * M_PI) + .5);
    return (1-easeValue) * _from + easeValue * _to;
}

#endif // FADEANIMATION_H
