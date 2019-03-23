#include "GroupAnimation.h"
#include "Animation.h"
#include <algorithm>

GroupAnimation::GroupAnimation(std::vector<AnimationBase*> group) : group_(group)
{
}

void GroupAnimation::Update(float elapsedTime)
{
  for (auto animation : group_)
  {
    animation->Update(elapsedTime);
  }
}

bool GroupAnimation::Active()
{
  return std::any_of(group_.begin(), group_.end(), [](AnimationBase* animation) { return animation->Active(); });
}
