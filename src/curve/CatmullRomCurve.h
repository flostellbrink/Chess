#ifndef CATMULL_ROM_CURVE_H
#define CATMULL_ROM_CURVE_H

#include "Curve.h"
#include "src/geometry/ControlPoints.h"

class CatmullRomCurve : public Curve
{
public:
  CatmullRomCurve(ControlPoints controlPoints, int curveId, int objectId);
  std::vector<glm::vec2> InterpolatedPoints(int resolution) override;
  glm::vec2 InterpolatedPoint(float value) override;
  std::vector<glm::vec2> InterpolatedTangents() override;
  glm::vec2 InterpolatedTangent(float value) override;

protected:
  ControlPoints control_points_;
  int object_id_, curve_id_;

private:
  int last_resolution_{};
};


#endif // CATMULL_ROM_CURVE_H
