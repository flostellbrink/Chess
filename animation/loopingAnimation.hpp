#ifndef LOOPINGANIMATION_H
#define LOOPINGANIMATION_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "animation.hpp"

template<class T>
class LoopingAnimation : public Animation<T>{
    public:
    LoopingAnimation(Animation<T>* looping);
    virtual T ValueAt(float value) override;

    protected:
    virtual float Value() override;
    Animation<T>* _looping;
    // Animation class needs some value for constructor
    T _fakeValue;

    private:
};

//Same as in animation.hpp
template<class T>
LoopingAnimation<T>::LoopingAnimation(Animation<T>* looping)
    : Animation<T>(looping->GetDuration(), _fakeValue){
    _looping = looping;
}

template<class T>
T LoopingAnimation<T>::ValueAt(float value){
    // usually animations use this value to set the property. simulate this behaviour:
    T result = _looping->ValueAt(value);
    _looping->SetValue(result);
    return result;
}

template<class T>
float LoopingAnimation<T>::Value(){
    float result = (float)this->_elapsedTotal / (float)this->_duration;
    if(result > 1){
        this->_elapsedTotal = 0;
        return 1;
    }
    return result;
}

#endif // LOOPINGANIMATION_H
