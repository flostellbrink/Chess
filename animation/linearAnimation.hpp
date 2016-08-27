#ifndef LINEARANIMATION_H
#define LINEARANIMATION_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "animation.hpp"

template<class T>
class LinearAnimation : public Animation<T>{
    public:
    LinearAnimation(int duration, T& property, T from, T to);
    virtual T ValueAt(float value) override;

    protected:
    T _from, _to;

    private:
};

//Same as in animation.hpp
template<class T>
LinearAnimation<T>::LinearAnimation(int duration, T& property, T from, T to)
    : Animation<T>(duration, property){
    _from = from;
    _to = to;
}

template<class T>
T LinearAnimation<T>::ValueAt(float value){
    return (1-value) * _from + value * _to;
}

#endif // LINEARANIMATION_H
