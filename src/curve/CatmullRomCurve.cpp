#include <cmath>
#include "CatmullRomCurve.h"

CatmullRomCurve::CatmullRomCurve(const ControlPoints controlPoints, const int curveId, const int objectId) {
  controlPoints_ = controlPoints;
  curveID_ = curveId;
  objectID_ = objectId;
}

std::vector<glm::vec2> CatmullRomCurve::InterpolatedPoints(const int resolution) {
  lastResolution_ = resolution;
  std::vector<glm::vec2> result(resolution);
  const auto stepSize = 1.0f / static_cast<float>(resolution - 1);
  for (auto i = 0; i < resolution; ++i) {
    result.push_back(InterpolatedPoint(i * stepSize));
  }

  return result;
}

glm::vec2 CatmullRomCurve::InterpolatedPoint(float value) {
  auto controlPoints = controlPoints_.GetControlPoints2D(objectID_)[curveID_];

  // Figure out what segment we are in
  const auto segments = static_cast<int>(controlPoints.size()) / 3 - 1;
  value *= segments;
  auto segment = static_cast<int>(value);
  if (segment == segments) {
    segment--;
  }
  value -= segment;

  const auto startIndex = segment * 3 + 1;
  const auto point1 = controlPoints.at(startIndex);
  const auto point2 = controlPoints.at(startIndex + 1);
  const auto point3 = controlPoints.at(startIndex + 2);
  const auto point4 = controlPoints.at(startIndex + 3);

  // Calculate the result using de Casteljau
  return  point1 * static_cast<float>(pow(1 - value, 3))
    + point2 * static_cast<float>(3 * pow(1 - value, 2) * value)
    + point3 * static_cast<float>(3 * (1 - value) * pow(value, 2))
    + point4 * static_cast<float>(pow(value, 3));
}

std::vector<glm::vec2> CatmullRomCurve::InterpolatedTangents() {
  std::vector<glm::vec2> result(lastResolution_);
  const auto stepSize = 1.0f / static_cast<float>(lastResolution_ - 1);
  for (auto i = 0; i < lastResolution_; ++i) {
    result.push_back(InterpolatedTangent(i * stepSize));
  }

  return result;
}

glm::vec2 CatmullRomCurve::InterpolatedTangent(float value) {
  auto controlPoints = controlPoints_.GetControlPoints2D(objectID_)[curveID_];

  // Figure out what segment we are in
  const auto segments = static_cast<int>(controlPoints.size()) / 3 - 1;
  value *= segments;
  auto segment = static_cast<int>(value);
  if (segment == segments) {
    segment--;
  }

  value -= segment;
  const auto startIndex = segment * 3 + 1;
  const auto point1 = controlPoints.at(startIndex);
  const auto point2 = controlPoints.at(startIndex + 1);
  const auto point3 = controlPoints.at(startIndex + 2);
  const auto point4 = controlPoints.at(startIndex + 3);

  // Calculate the result using first derivative of de Casteljau
  return  3.f * ((point4 - 3.f * point3 + 3.f * point2 - point1) * static_cast<float>(pow(value, 2))
    + (2.f * point3 - 4.f *point2 + 2.f*point1) * value + point2 - point1);
}
