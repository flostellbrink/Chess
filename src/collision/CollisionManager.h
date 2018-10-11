#ifndef COLLMANAGER_H
#define COLLMANAGER_H

#include <glm/glm.hpp>

class Collision;

class CollisionManager {
public:
  CollisionManager();
  static Collision* GetAABB(glm::vec3 min, glm::vec3 max);
  static Collision* GetRay(glm::vec3 from, glm::vec3 to);
};


#endif // COLLMANAGER_H
