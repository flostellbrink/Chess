#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.inl>
#include <cmath>

#include "LineCurve.h"

LineCurve::LineCurve(const float radius) : radius_(radius)
{
}

std::vector<glm::vec2> LineCurve::InterpolatedPoints(const int resolution)
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

glm::vec2 LineCurve::InterpolatedPoint(float value)
{
  // Move value into -1 1 range
  value = (value - 0.5f) * 2;
  return glm::vec2(value * 2, cos(value * glm::pi<float>() * radius_));
}

std::vector<glm::vec2> LineCurve::InterpolatedTangents()
{
  return tangents_;
}

glm::vec2 LineCurve::InterpolatedTangent(float value)
{
  // Move value into -1 1 range
  value = (value - 0.5f) * 2;
  value *= glm::pi<float>() * radius_;

  // Shift by a quarter rotation
  value -= glm::pi<float>() * 0.25f;
  return glm::vec2(sin(value), cos(value));
}
