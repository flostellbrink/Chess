#ifndef DELAY_ANIMATION_H
#define DELAY_ANIMATION_H

#include "AnimationBase.h"

template <class T>
class DelayAnimation : public AnimationBase
{
public:
  explicit DelayAnimation(float duration);
  void Update(float elapsedTime) override;
  bool Active() override;

private:
  float duration_, elapsed_total_ = 0;
  bool active_ = true;
};

template <class T>
DelayAnimation<T>::DelayAnimation(const float duration) : duration_(duration)
{
}

template <class T>
void DelayAnimation<T>::Update(const float elapsedTime)
{
  elapsed_total_ += elapsedTime;
  if (elapsed_total_ >= duration_)
  {
    active_ = false;
  }
}

template <class T>
bool DelayAnimation<T>::Active()
{
  return active_;
}

#endif // DELAY_ANIMATION_H
