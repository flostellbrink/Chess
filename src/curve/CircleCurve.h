#ifndef CIRCLECURVE_H
#define CIRCLECURVE_H

#include <glm/glm.hpp>
#include "Curve.h"

class CircleCurve : public Curve {
public:
  explicit CircleCurve(float radius = 1.f);
  std::vector<glm::vec2> InterpolatedPoints(int resolution) override;
  glm::vec2 InterpolatedPoint(float value) override;
  std::vector<glm::vec2> InterpolatedTangents() override;
  glm::vec2 InterpolatedTangent(float value) override;

protected:
  float radius_;
};

#endif // CIRCLECURVE_H
