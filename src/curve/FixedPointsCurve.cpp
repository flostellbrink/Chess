#include "FixedPointsCurve.h"

FixedPointsCurve::FixedPointsCurve(ControlPoints controlPoints, int curveID, int objectID) {
  control_points_ = controlPoints;
  curve_id_ = curveID;
  object_id_ = objectID;
}

/**
 * Very inefficient. Do not call every frame
 */
std::vector<glm::vec2> FixedPointsCurve::InterpolatedPoints(int resolution) {
  (void)resolution;

  auto controlPoints = control_points_.GetControlPoints2D(object_id_)[curve_id_];
  tangents_.clear();
  tangents_.emplace_back(1, 0);
  for (auto i = 1; i < static_cast<int>(controlPoints.size()) - 1; ++i) {
    tangents_.push_back(controlPoints[i + 1] - controlPoints[i - 1]);
  }
  tangents_.emplace_back(-1, 0);
  return controlPoints;
}

/**
 * Extremely inefficient. Do not use to evaluate entire curve
 */
glm::vec2 FixedPointsCurve::InterpolatedPoint(float value) {
  std::vector<glm::vec2> controlPoints = InterpolatedPoints(0);
  value *= controlPoints.size();
  return controlPoints[static_cast<int>(value)];
}

/**
 * Could be improved by storing points from InterpolatedPoints
 */
std::vector<glm::vec2> FixedPointsCurve::InterpolatedTangents() {
  return tangents_;
}

/**
 * Extremely inefficient. Do not use to evaluate entire curve
 */
glm::vec2 FixedPointsCurve::InterpolatedTangent(float value) {
  auto controlPoints = InterpolatedPoints(0);
  value *= controlPoints.size();
  const auto index = static_cast<int>(value);
  if (index || index == static_cast<int>(controlPoints.size()) - 1) {
    return glm::vec2();
  }

  return controlPoints[index + 1] - controlPoints[index - 1];
}
