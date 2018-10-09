#include <GL/glew.h>

#include "collision.hpp"
#include "glbase/gltool.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "objects/objectManager.hpp"

using std::min;
using std::max;

Collision::Collision(int type, glm::vec3 vecA, glm::vec3 vecB){
    Type = type;
    switch (type) {
    case Types::AABB:
        min = vecA;
        max = vecB;
        break;
    case Types::Ray:
        origin = vecA;
        dir = glm::normalize(vecB - vecA);
        minLength = 0;
        maxLength = glm::length(vecB - vecA);
        break;
    default:
        std::cerr << "ChessWarn: Unknown collision type: " << type << std::endl;
        break;
    }
}

bool AabbCollAabb(Collision* a, Collision *b){
    return (a->max.x >= b->min.x && b->max.x >= a->min.x
        &&  a->max.y >= b->min.y && b->max.y >= a->min.y
        &&  a->max.z >= b->min.z && b->max.z >= a->min.z);
}

bool RayCollRay(Collision* a, Collision* b){
    (void)a;
    (void)b;
    // TODO implement if needed
    std::cerr << "ChessWarn: Ray Ray collision not handled" << std::endl;
    return false;
}

bool AabbCollRay(Collision* a, Collision* b, float minLength, float maxLength){
    float t1 = (a->min.x - b->origin.x) / b->dir.x,
        t2 = (a->max.x - b->origin.x) / b->dir.x,
        t3 = (a->min.y - b->origin.y) / b->dir.y,
        t4 = (a->max.y - b->origin.y) / b->dir.y,
        t5 = (a->min.z - b->origin.z) / b->dir.z,
        t6 = (a->max.z - b->origin.z) / b->dir.z;

    float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
    float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

    return tmax >= tmin && tmax >= minLength && tmin <= maxLength;
}

bool Collision::Intersects(Collision* other){
    if(Type == Types::AABB && other->Type == Types::AABB){
        return AabbCollAabb(this, other);
    } else if(Type == Types::Ray && other->Type == Types::Ray){
        return RayCollRay(this, other);
    } else if(Type == Types::AABB && other->Type == Types::Ray){
        return AabbCollRay(this, other, other->minLength, other->maxLength);
    }else if(Type == Types::Ray && other->Type == Types::AABB){
        return AabbCollRay(other, this, minLength, maxLength);
    }  else {
        std::cerr << "ChessWarn: Unknown collsion between: " << Type << " and: " << other->Type << std::endl;
        return false;
    }
}

bool Collision::Contains(glm::vec3 point){
    switch (Type) {
        case Types::AABB:
            return point.x <= max.x && point.x >= min.x
                && point.y <= max.y && point.y >= min.y
                && point.z <= max.z && point.z >= min.z;
        case Types::Ray:
            // Should not be used, won't work. Not practical due to precision
            std::cerr << "ChessWarn: Point Ray collision not handled" << std::endl;
            return false;
        default:break;
    }
    std::cerr << "ChessWarn: Unknown collision type: " << Type << std::endl;
    return false;
}
