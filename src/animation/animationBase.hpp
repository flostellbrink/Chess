#ifndef ANIMATIONBASE_H
#define ANIMATIONBASE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Provides an interface for animations
class AnimationBase{
    public:
    virtual void Update(float elapsedTime) = 0;
    virtual bool Active() = 0;

    protected:

    private:
};

#endif // ANIMATIONBASE_H
