#ifndef DELAYANIMATION_H
#define DELAYANIMATION_H

#include "AnimationBase.h"

template<class T>
class DelayAnimation : public AnimationBase {
public:
  DelayAnimation(float duration);
  void Update(float elapsedTime) override;
  bool Active() override;

private:
  float duration_, elapsedTotal_ = 0;
  bool active_ = true;
};

template<class T>
DelayAnimation<T>::DelayAnimation(float duration) : duration_(duration) {

}

template<class T>
void DelayAnimation<T>::Update(const float elapsedTime) {
  elapsedTotal_ += elapsedTime;
  if(elapsedTotal_ >= duration_)
  {
    active_ = false;
  }
}

template<class T>
bool DelayAnimation<T>::Active() {
  return active_;
}

#endif // DELAYANIMATION_H
