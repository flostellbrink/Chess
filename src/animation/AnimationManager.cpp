#include "AnimationManager.h"
#include <algorithm>
#include "AnimationBase.h"

AnimationManager::AnimationManager() = default;

void AnimationManager::Update(const float elapsedTime)
{
  for (auto animation : active_animations_)
  {
    animation->Update(elapsedTime);
    if (!animation->Active())
    {
      finished_animations_.push_back(animation);
    }
  }

  for (auto animation : finished_animations_)
  {
    active_animations_.erase(std::remove(active_animations_.begin(), active_animations_.end(), animation),
                             active_animations_.end());
    delete animation;
  }
  finished_animations_.clear();

  for (auto animation : independent_animations_)
  {
    animation->Update(elapsedTime);
    if (!animation->Active())
    {
      finished_animations_.push_back(animation);
    }
  }

  for (auto animation : finished_animations_)
  {
    independent_animations_.erase(
      std::remove(independent_animations_.begin(), independent_animations_.end(), animation),
      independent_animations_.end());
    delete animation;
  }
  finished_animations_.clear();

  if (active_animations_.empty() && !queued_animations_.empty())
  {
    active_animations_.push_back(queued_animations_[0]);
    queued_animations_.erase(queued_animations_.begin());
  }
}

void AnimationManager::PlayNow(AnimationBase* animation)
{
  active_animations_.push_back(animation);
}

void AnimationManager::PlayNext(AnimationBase* animation)
{
  queued_animations_.insert(queued_animations_.begin(), animation);
}

void AnimationManager::PlayLast(AnimationBase* animation)
{
  queued_animations_.push_back(animation);
}

void AnimationManager::PlayIndependent(AnimationBase* animation)
{
  independent_animations_.push_back(animation);
}

bool AnimationManager::IsBusy() const
{
  return !active_animations_.empty() || !queued_animations_.empty();
}

void AnimationManager::Reset()
{
  for (auto animation : active_animations_)
  {
    delete animation;
  }
  active_animations_.clear();

  for (auto animation : queued_animations_)
  {
    delete animation;
  }
  queued_animations_.clear();
}
