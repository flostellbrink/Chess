#ifndef LINEAR_ANIMATION_H
#define LINEAR_ANIMATION_H

#include "Animation.h"

template<class T>
class LinearAnimation : public Animation<T> {
public:
  LinearAnimation(float duration, T& property, T from, T to);
  T ValueAt(float value) override;

protected:
  T from_, to_;
};

//Same as in animation.h
template<class T>
LinearAnimation<T>::LinearAnimation(const float duration, T& property, T from, T to)
  : Animation<T>(duration, property) {
  from_ = from;
  to_ = to;
}

template<class T>
T LinearAnimation<T>::ValueAt(float value) {
  return (1 - value) * from_ + value * to_;
}

#endif // LINEAR_ANIMATION_H
