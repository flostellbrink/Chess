#include <GL/glew.h>

#include "aniManager.hpp"
#include <iostream>
#include <algorithm>

AniManager::AniManager()
{

}

void AniManager::Update(int elapsedTime){
    for(auto anim : _activeAnimations){
        anim->Update(elapsedTime);
        if(!anim->Active())
            _finishedAnimations.push_back(anim);
    }
    for(auto anim : _finishedAnimations)
        _activeAnimations.erase(std::remove(_activeAnimations.begin(), _activeAnimations.end(), anim), _activeAnimations.end());
    _finishedAnimations.clear();

    for(auto anim : _independentAnimations){
        anim->Update(elapsedTime);
        if(!anim->Active())
            _finishedAnimations.push_back(anim);
    }
    for(auto anim : _finishedAnimations)
        _independentAnimations.erase(std::remove(_independentAnimations.begin(), _independentAnimations.end(), anim), _independentAnimations.end());
    _finishedAnimations.clear();

    if(_activeAnimations.size() == 0 && _queuedAnimations.size() > 0){
		_activeAnimations.push_back(_queuedAnimations[0]);
        _queuedAnimations.erase(_queuedAnimations.begin());
    }
}

void AniManager::PlayNow(AnimationBase *anim){
    _activeAnimations.push_back(anim);
}

void AniManager::PlayNext(AnimationBase *anim){
    _queuedAnimations.insert(_queuedAnimations.begin(), anim);
}

void AniManager::PlayLast(AnimationBase *anim){
    _queuedAnimations.push_back(anim);
}

void AniManager::PlayIndependent(AnimationBase *anim){
    _independentAnimations.push_back(anim);
}

bool AniManager::isBusy(){
    return _activeAnimations.size() || _queuedAnimations.size();
}

void AniManager::Reset(){
    _activeAnimations.clear();
    _queuedAnimations.clear();
}
