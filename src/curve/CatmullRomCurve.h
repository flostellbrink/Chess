#ifndef CATMULLROMCURVE_H
#define CATMULLROMCURVE_H

#include "Curve.h"
#include "src/geometry/ControlPoints.h"

class CatmullRomCurve : public Curve {
public:
  CatmullRomCurve(ControlPoints controlPoints, int curveId, int objectId);
  std::vector<glm::vec2> InterpolatedPoints(int resolution) override;
  glm::vec2 InterpolatedPoint(float value) override;
  std::vector<glm::vec2> InterpolatedTangents() override;
  glm::vec2 InterpolatedTangent(float value) override;

protected:
  ControlPoints controlPoints_;
  int objectID_, curveID_;

private:
  int lastResolution_{};
};


#endif // CATMULLROMCURVE_H
