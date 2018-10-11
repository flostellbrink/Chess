#ifndef REVOLVER_H
#define REVOLVER_H

#include "src/geometry/Geometry.h"

class Curve;

class Revolver : public Geometry {
public:
  Revolver(Curve* baseCurve, Curve* heightCurve, float scale = .1, bool clockTexture = false);

protected:
  void Create() override;

private:
  Curve* base_curve_;
  Curve* height_curve_;
  float scale_;
  bool clock_texture_;
};


#endif // REVOLVER_H
