#ifndef LOOPING_ANIMATION_H
#define LOOPING_ANIMATION_H

#include "Animation.h"

template <class T>
class LoopingAnimation : public Animation<T>
{
public:
  explicit LoopingAnimation(Animation<T>* looping);
  T ValueAt(float value) override;

protected:
  float Value() override;
  Animation<T>* looping_;
  // Animation class needs some value for constructor
  T fake_value_;
};

//Same as in animation.h
template <class T>
LoopingAnimation<T>::LoopingAnimation(Animation<T>* looping)
  : Animation<T>(looping->GetDuration(), fake_value_)
{
  looping_ = looping;
}

template <class T>
T LoopingAnimation<T>::ValueAt(float value)
{
  // usually animations use this value to set the property. simulate this behaviour:
  T result = looping_->ValueAt(value);
  looping_->SetValue(result);
  return result;
}

template <class T>
float LoopingAnimation<T>::Value()
{
  const float result = this->elapsed_total_ / this->duration_;
  if (result > 1)
  {
    this->elapsed_total_ = 0;
    return 1;
  }
  return result;
}

#endif // LOOPING_ANIMATION_H
