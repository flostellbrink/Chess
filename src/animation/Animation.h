#ifndef ANIMATION_H
#define ANIMATION_H

#include "AnimationBase.h"

template <class T>
class Animation : public AnimationBase
{
public:
  Animation(float duration, T& property);
  void Update(float elapsedTime) override;
  bool Active() override;
  virtual T ValueAt(float value) = 0;
  float GetDuration() const;
  void SetValue(T value);

protected:
  virtual float Value();
  float duration_, elapsed_total_ = 0;
  T& property_;
  bool active_ = true;
};

template <class T>
Animation<T>::Animation(const float duration, T& property) : duration_(duration), property_(property)
{
}

template <class T>
void Animation<T>::Update(const float elapsedTime)
{
  elapsed_total_ += elapsedTime;
  property_ = ValueAt(Value());
}

template <class T>
bool Animation<T>::Active()
{
  return active_;
}

template <class T>
float Animation<T>::Value()
{
  const auto result = elapsed_total_ / duration_;
  if (result > 1)
  {
    active_ = false;
    return 1;
  }
  return result;
}

template <class T>
float Animation<T>::GetDuration() const
{
  return duration_;
}

template <class T>
void Animation<T>::SetValue(T value)
{
  property_ = value;
}

#endif // ANIMATION_H
