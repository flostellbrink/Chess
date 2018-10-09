#ifndef ANIMATIONBASE_H
#define ANIMATIONBASE_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>

// Provides an interface for animations
class AnimationBase{
    public:
    virtual void Update(float elapsedTime) = 0;
    virtual bool Active() = 0;

    protected:

    private:
};

#endif // ANIMATIONBASE_H
