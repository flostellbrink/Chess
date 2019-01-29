#ifndef ANIMATION_BASE_H
#define ANIMATION_BASE_H

// Provides an interface for animations
class AnimationBase
{
public:
  virtual ~AnimationBase() = default;
  virtual void Update(float elapsedTime) = 0;
  virtual bool Active() = 0;
};

#endif // ANIMATION_BASE_H
