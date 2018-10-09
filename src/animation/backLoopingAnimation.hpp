#ifndef BACKLOOPINGANIMATION_H
#define BACKLOOPINGANIMATION_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "animation.hpp"

// Like looping, but also runs animation in reverse

template<class T>
class BackLoopingAnimation : public Animation<T>{
    public:
    BackLoopingAnimation(Animation<T>* looping);
    virtual T ValueAt(float value) override;

    protected:
    virtual float Value() override;
    Animation<T>* _looping;
    // Animation class needs some value for constructor
    T _fakeValue;
    bool forward = true;

    private:
};

//Same as in animation.hpp
template<class T>
BackLoopingAnimation<T>::BackLoopingAnimation(Animation<T>* looping)
    : Animation<T>(looping->GetDuration(), _fakeValue){
    _looping = looping;
}

template<class T>
T BackLoopingAnimation<T>::ValueAt(float value){
    // usually animations use this value to set the property. simulate this behaviour:
    T result = _looping->ValueAt(forward ? value : 1 - value);
    _looping->SetValue(result);
    return result;
}

template<class T>
float BackLoopingAnimation<T>::Value(){
    float result = (float)this->_elapsedTotal / (float)this->_duration;
    if(result > 1){
        this->_elapsedTotal = 0;
        forward = !forward;
        return 0;
    }
    return result;
}

#endif // BACKLOOPINGANIMATION_H
