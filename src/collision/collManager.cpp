#include <GL/glew.h>
#include <algorithm>
#include <iostream>

#include "collManager.hpp"
#include "src/objects.hpp"
#include "src/curve/fixedPointsCurve.hpp"
#include "src/curve/circleCurve.hpp"
#include "src/curve/catmullRomCurve.hpp"
#include "src/curve/lineCurve.hpp"

CollManager::CollManager(){

}

Collision* CollManager::GetAABB(glm::vec3 min, glm::vec3 max){
    return new Collision(Collision::AABB, min, max);
}

Collision* CollManager::GetRay(glm::vec3 from, glm::vec3 to){
    return new Collision(Collision::Ray, from, to);
}
