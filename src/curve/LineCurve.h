#ifndef LINE_CURVE_H
#define LINE_CURVE_H

#include "Curve.h"

class LineCurve : public Curve {
public:
  explicit LineCurve(float radius = .2f);
  std::vector<glm::vec2> InterpolatedPoints(int resolution) override;
  glm::vec2 InterpolatedPoint(float value) override;
  std::vector<glm::vec2> InterpolatedTangents() override;
  glm::vec2 InterpolatedTangent(float value) override;

protected:
  float radius_;
};

#endif // LINE_CURVE_H
