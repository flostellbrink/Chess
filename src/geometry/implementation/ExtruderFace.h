#ifndef EXTRUDER_FACE_H
#define EXTRUDER_FACE_H

#include "src/geometry/Geometry.h"

class Curve;

class ExtruderFace : public Geometry {
public:
  ExtruderFace(Curve* profileCurve, Curve* compressedProfile, Curve* widthCurve, float widthPos, float scale = .1f);

protected:
  void Create() override;

private:
  Curve* profile_curve_;
  Curve* width_curve_;
  Curve* compressed_profile_;
  float scale_, width_pos_;
};


#endif // EXTRUDER_FACE_H
