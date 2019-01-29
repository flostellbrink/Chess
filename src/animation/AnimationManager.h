#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include <vector>

class AnimationBase;

class AnimationManager
{
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
  std::vector<AnimationBase*> active_animations_, queued_animations_, independent_animations_, finished_animations_;
};


#endif // ANIMATION_MANAGER_H
