// Loads geometry for all objects
#ifndef COLLISION_H
#define COLLISION_H

#include <GL/glew.h>
#include <glm/common.hpp>

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include <vector>

using std::vector;
using glm::vec2;
using glm::vec3;

class Collision{
    public:
    Collision(int type, vec3 vecA, vec3 vecB);
    bool Intersects(Collision* other);
    bool Contains(vec3 point);

    int Type;
    enum Types {AABB, Ray};
    vec3 min, max, origin, dir;
    float minLength, maxLength;

    protected:

    private:
};


#endif // COLLISION_H
