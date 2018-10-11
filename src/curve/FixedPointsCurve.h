#ifndef FIXEDPOINTSCURVE_H
#define FIXEDPOINTSCURVE_H

#include "Curve.h"
#include "src/geometry/ControlPoints.h"

class FixedPointsCurve : public Curve {
public:
  FixedPointsCurve(ControlPoints controlPoints, int curveID, int objectID);
  std::vector<glm::vec2> InterpolatedPoints(int resolution) override;
  glm::vec2 InterpolatedPoint(float value) override;
  std::vector<glm::vec2> InterpolatedTangents() override;
  glm::vec2 InterpolatedTangent(float value) override;

protected:
  ControlPoints controlPoints_;
  int objectID_, curveID_;
};


#endif // FIXEDPOINTSCURVE_H
