#ifndef FADE_ANIMATION_H
#define FADE_ANIMATION_H

#include <glm/glm.hpp>

#include "Animation.h"

template <class T>
class FadeAnimation : public Animation<T>
{
public:
  FadeAnimation(float duration, T& property, T from, T to);
  T ValueAt(float value) override;

protected:
  T from_, to_;
};

//Same as in animation.h
template <class T>
FadeAnimation<T>::FadeAnimation(const float duration, T& property, T from, T to)
  : Animation<T>(duration, property)
{
  from_ = from;
  to_ = to;
}

template <class T>
T FadeAnimation<T>::ValueAt(const float value)
{
  auto easeValue = static_cast<float>(
    0.5f * sin(value * glm::pi<float>() - 0.5f * glm::pi<float>()) + 0.5f);

  return (1 - easeValue) * from_ + easeValue * to_;
}

#endif // FADE_ANIMATION_H
