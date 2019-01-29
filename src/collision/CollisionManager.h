#ifndef COLL_MANAGER_H
#define COLL_MANAGER_H

#include <glm/glm.hpp>

class Collision;

class CollisionManager
{
public:
  CollisionManager();
  static Collision* GetAabb(glm::vec3 min, glm::vec3 max);
  static Collision* GetRay(glm::vec3 from, glm::vec3 to);
};


#endif // COLL_MANAGER_H
