#ifndef COLLMANAGER_H
#define COLLMANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "collision.hpp"

using std::vector;

class CollManager{
    public:
    CollManager();
    Collision* GetAABB(vec3 min, vec3 max);
    Collision* GetRay(vec3 from, vec3 to);
    void Reset();

    protected:

    private:
};


#endif // COLLMANAGER_H