#include "AnimationManager.h"
#include <algorithm>
#include "AnimationBase.h"

AnimationManager::AnimationManager() = default;

void AnimationManager::Update(const float elapsedTime) {
  for (auto animation : activeAnimations_) {
    animation->Update(elapsedTime);
    if (!animation->Active()) {
      finishedAnimations_.push_back(animation);
    }
  }

  for (auto animation : finishedAnimations_) {
    activeAnimations_.erase(std::remove(activeAnimations_.begin(), activeAnimations_.end(), animation), activeAnimations_.end());
  }
  finishedAnimations_.clear();

  for (auto animation : independentAnimations_) {
    animation->Update(elapsedTime);
    if (!animation->Active()) {
      finishedAnimations_.push_back(animation);
    }
  }

  for (auto animation : finishedAnimations_) {
    independentAnimations_.erase(std::remove(independentAnimations_.begin(), independentAnimations_.end(), animation), independentAnimations_.end());
  }
  finishedAnimations_.clear();

  if (activeAnimations_.empty() && !queuedAnimations_.empty()) {
    activeAnimations_.push_back(queuedAnimations_[0]);
    queuedAnimations_.erase(queuedAnimations_.begin());
  }
}

void AnimationManager::PlayNow(AnimationBase *animation) {
  activeAnimations_.push_back(animation);
}

void AnimationManager::PlayNext(AnimationBase *animation) {
  queuedAnimations_.insert(queuedAnimations_.begin(), animation);
}

void AnimationManager::PlayLast(AnimationBase *animation) {
  queuedAnimations_.push_back(animation);
}

void AnimationManager::PlayIndependent(AnimationBase *animation) {
  independentAnimations_.push_back(animation);
}

bool AnimationManager::IsBusy() const
{
  return !activeAnimations_.empty() || !queuedAnimations_.empty();
}

void AnimationManager::Reset() {
  activeAnimations_.clear();
  queuedAnimations_.clear();
}
