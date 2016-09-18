#ifndef ANIMATION_H
#define ANIMATION_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "animationBase.hpp"

template<class T>
class Animation : public AnimationBase{
    public:
    Animation(float duration, T &property);
    void Update(float elapsedTime) override;
    virtual bool Active() override;
    virtual T ValueAt(float value) = 0;
    float GetDuration();
    void SetValue(T value);

    protected:
    virtual float Value();
    float _duration, _elapsedTotal = 0;
    T& _property;
    bool _active = true;

    private:
};

//Apparently template classes functions cannot be implemented in cpp files..
template<class T>
Animation<T>::Animation(float duration, T &property) : _duration(duration), _property(property) {

}

template<class T>
void Animation<T>::Update(float elapsedTime){
    _elapsedTotal += elapsedTime;
    _property = ValueAt(Value());
}

template<class T>
bool Animation<T>::Active(){
    return _active;
}

template<class T>
float Animation<T>::Value(){
    float result = _elapsedTotal / _duration;
    if(result > 1){
        _active = false;
        return 1;
    }
    return result;
}

template<class T>
float Animation<T>::GetDuration(){
    return _duration;
}

template<class T>
void Animation<T>::SetValue(T value){
    _property = value;
}

#endif // ANIMATION_H
