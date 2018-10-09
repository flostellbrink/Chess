#ifndef ANIMANAGER_H
#define ANIMANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "animationBase.hpp"
#include "animation.hpp"

using std::vector;

class AniManager{
    public:
    AniManager();
    void Update(float elapsedTime);
    void PlayNow(AnimationBase* anim);
    void PlayNext(AnimationBase* anim);
    void PlayLast(AnimationBase* anim);
    void PlayIndependent(AnimationBase* anim);
    bool isBusy();
    void Reset();

    protected:
    vector<AnimationBase*> _activeAnimations, _queuedAnimations, _independentAnimations, _finishedAnimations;

    private:
};


#endif // ANIMANAGER_H
