#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include "CircleCurve.h"


CircleCurve::CircleCurve(const float radius) : radius_(radius)
{
}

std::vector<glm::vec2> CircleCurve::InterpolatedPoints(const int resolution)
{
  const auto stepSize = 1.f / (resolution - 1);
  std::vector<glm::vec2> result;
  tangents_.clear();
  for (auto i = 0; i < resolution; ++i)
  {
    const auto value = i * stepSize;
    result.push_back(InterpolatedPoint(value));
    tangents_.push_back(InterpolatedTangent(value));
  }

  return result;
}

glm::vec2 CircleCurve::InterpolatedPoint(float value)
{
  value *= glm::pi<float>() * 2.f;
  return radius_ * glm::vec2(cos(value), sin(value));
}

std::vector<glm::vec2> CircleCurve::InterpolatedTangents()
{
  return tangents_;
}

glm::vec2 CircleCurve::InterpolatedTangent(float value)
{
  value -= 0.25f;
  value *= glm::pi<float>() * 2.f;
  return glm::vec2(cos(value), sin(value));
}
