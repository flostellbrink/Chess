#ifndef BACK_LOOPING_ANIMATION_H
#define BACK_LOOPING_ANIMATION_H

// Like looping, but also runs animation in reverse

template <class T>
class BackLoopingAnimation : public Animation<T>
{
public:
  explicit BackLoopingAnimation(Animation<T>* looping);
  T ValueAt(float value) override;

protected:
  float Value() override;
  Animation<T>* looping_;
  // Animation class needs some value for constructor
  T fake_value_;
  bool forward_ = true;
};

//Same as in animation.h
template <class T>
BackLoopingAnimation<T>::BackLoopingAnimation(Animation<T>* looping)
  : Animation<T>(looping->GetDuration(), fake_value_)
{
  looping_ = looping;
}

template <class T>
T BackLoopingAnimation<T>::ValueAt(const float value)
{
  // usually animations use this value to set the property. simulate this behaviour:
  T result = looping_->ValueAt(forward_ ? value : 1 - value);
  looping_->SetValue(result);
  return result;
}

template <class T>
float BackLoopingAnimation<T>::Value()
{
  const float result = this->elapsed_total_ / this->duration_;
  if (result > 1)
  {
    this->elapsed_total_ = 0;
    forward_ = !forward_;
    return 0;
  }
  return result;
}

#endif // BACK_LOOPING_ANIMATION_H
