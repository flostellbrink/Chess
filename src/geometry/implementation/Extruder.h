#ifndef EXTRUDER_H
#define EXTRUDER_H

#include "src/geometry/Geometry.h"

class Curve;

class Extruder : public Geometry {
public:
  Extruder(Curve* profileCurve, Curve* widthCurve, float scale = .1f);

protected:
  void Create() override;

private:
  Curve* profile_curve_;
  Curve* width_curve_;
  float scale_;
  int resolution_ = 10;
};


#endif // EXTRUDER_H
