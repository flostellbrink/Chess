#ifndef ANIMATIONBASE_H
#define ANIMATIONBASE_H

// Provides an interface for animations
class AnimationBase {
public:
  virtual ~AnimationBase() = default;
  virtual void Update(float elapsedTime) = 0;
  virtual bool Active() = 0;
};

#endif // ANIMATIONBASE_H
