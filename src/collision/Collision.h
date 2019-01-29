// Loads geometry for all objects
#ifndef COLLISION_H
#define COLLISION_H

#include <glm/glm.hpp>

class Collision
{
public:
  Collision(int type, glm::vec3 vecA, glm::vec3 vecB);
  bool Intersects(Collision* other);
  bool Contains(glm::vec3 point) const;

  int type{};

  enum types { aabb, ray };

  glm::vec3 min, max, origin, dir;
  float min_length, max_length;
};


#endif // COLLISION_H
