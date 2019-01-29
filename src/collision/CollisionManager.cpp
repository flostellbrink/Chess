#include <GL/glew.h>
#include <algorithm>

#include "CollisionManager.h"
#include "Collision.h"
#include "src/curve/CircleCurve.h"

CollisionManager::CollisionManager() = default;

Collision* CollisionManager::GetAabb(const glm::vec3 min, const glm::vec3 max)
{
  return new Collision(Collision::aabb, min, max);
}

Collision* CollisionManager::GetRay(const glm::vec3 from, const glm::vec3 to)
{
  return new Collision(Collision::ray, from, to);
}
