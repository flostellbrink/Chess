#ifndef GROUP_ANIMATION_H
#define GROUP_ANIMATION_H

#include <vector>
#include "AnimationBase.h"

// Runs a group of animations at the same time. Finishes when all animations have finished
class GroupAnimation : public AnimationBase
{
public:
  GroupAnimation(std::vector<AnimationBase*> group);
  void Update(float elapsedTime) override;
  bool Active() override;

protected:
  std::vector<AnimationBase*> group_;
};

#endif // GROUP_ANIMATION_H
