#ifndef INTERLACEDREVOLVER_H
#define INTERLACEDREVOLVER_H

#include "src/geometry/Geometry.h"

class Curve;

class InterlacedRevolver : public Geometry {
public:
  /**
   * @brief InterlacedRevolver Used for the knight, toggles between two height profiles at given length values
   * @param baseCurve Curve for x and z axis
   * @param heightCurve1 First Curve for y axis
   * @param heightCurve2 Second Curve for y axis
   * @param length1 Size of First Curves Segments
   * @param length2 Size of Second Curves Segments
   */
  InterlacedRevolver(Curve* baseCurve, Curve* heightCurve1, Curve* heightCurve2, float length1, float length2, float scale = .1);

protected:
  void Create() override;

private:
  Curve* base_curve_;
  Curve* height_curve1_;
  Curve* height_curve2_;
  float scale_, length1_, length2_;
};


#endif // INTERLACEDREVOLVER_H
