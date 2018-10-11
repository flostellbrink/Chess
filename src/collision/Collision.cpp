#include <glm/glm.hpp>
#include <algorithm>
#include <iostream>
#include <src/Logger.h>
#include "src/objects/ObjectManager.h"
#include "Collision.h"

Collision::Collision(int type, glm::vec3 vecA, glm::vec3 vecB) {
  Collision::type = type;
  switch (type) {
  case aabb:
    min = vecA;
    max = vecB;
    break;
  case ray:
    origin = vecA;
    dir = normalize(vecB - vecA);
    min_length = 0;
    max_length = length(vecB - vecA);
    break;
  default:
    std::cerr << "ChessWarn: Unknown collision type: " << type << std::endl;
    break;
  }
}

bool aabb_coll_aabb(Collision* a, Collision *b) {
  return (a->max.x >= b->min.x && b->max.x >= a->min.x
    &&  a->max.y >= b->min.y && b->max.y >= a->min.y
    &&  a->max.z >= b->min.z && b->max.z >= a->min.z);
}

bool ray_coll_ray(Collision* a, Collision* b) {
  (void)a;
  (void)b;
  Logger::Error("ChessWarn: Ray Ray collision not handled");
  return false;
}

bool aabb_coll_ray(Collision* a, Collision* b, float minLength, float maxLength) {
  const auto t1 = (a->min.x - b->origin.x) / b->dir.x;
  const auto t2 = (a->max.x - b->origin.x) / b->dir.x;
  const auto t3 = (a->min.y - b->origin.y) / b->dir.y;
  const auto t4 = (a->max.y - b->origin.y) / b->dir.y;
  const auto t5 = (a->min.z - b->origin.z) / b->dir.z;
  const auto t6 = (a->max.z - b->origin.z) / b->dir.z;

  const auto tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
  const auto tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

  return tmax >= tmin && tmax >= minLength && tmin <= maxLength;
}

bool Collision::Intersects(Collision* other) {
  if (type == aabb && other->type == aabb) {
    return aabb_coll_aabb(this, other);
  }

  if (type == ray && other->type == ray) {
    return ray_coll_ray(this, other);
  }

  if (type == aabb && other->type == ray) {
    return aabb_coll_ray(this, other, other->min_length, other->max_length);
  }

  if (type == ray && other->type == aabb) {
    return aabb_coll_ray(other, this, min_length, max_length);
  }

  Logger::Error("ChessWarn: Unknown intersection");
  return false;
}

bool Collision::Contains(glm::vec3 point) const
{
  switch (type) {
  case aabb:
    return point.x <= max.x && point.x >= min.x
      && point.y <= max.y && point.y >= min.y
      && point.z <= max.z && point.z >= min.z;
  case ray:
    // Should not be used, won't work. Not practical due to precision
    Logger::Error("ChessWarn: Point Ray collision not handled");
    return false;
  default:break;
  }

  Logger::Error("ChessWarn: Unknown containment");
  return false;
}
