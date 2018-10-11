#ifndef ANIMANAGER_H
#define ANIMANAGER_H

#include <vector>

class AnimationBase;

class AnimationManager {
public:
  AnimationManager();
  void Update(float elapsedTime);
  void PlayNow(AnimationBase* animation);
  void PlayNext(AnimationBase* animation);
  void PlayLast(AnimationBase* animation);
  void PlayIndependent(AnimationBase* animation);
  bool IsBusy() const;
  void Reset();

protected:
  std::vector<AnimationBase*> activeAnimations_, queuedAnimations_, independentAnimations_, finishedAnimations_;
};


#endif // ANIMANAGER_H
