#ifndef FADEANIMATION_H
#define FADEANIMATION_H

#include <glm/gtx/transform.hpp>
#include "math.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
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
    auto easeValue = (float)(.5f * sin(value * glm::pi<float>() - .5f * glm::pi<float>()) + .5f);
    return (1-easeValue) * _from + easeValue * _to;
}

#endif // FADEANIMATION_H
