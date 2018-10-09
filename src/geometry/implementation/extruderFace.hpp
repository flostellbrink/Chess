#ifndef EXTRUDERFACE_H
#define EXTRUDERFACE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/geometry/geometry.hpp"
#include "src/curve/curve.hpp"

class ExtruderFace : public Geometry{
    public:
    ExtruderFace(Curve* profileCurve, Curve* compressedProfile, Curve* widthCurve, float widthPos, float scale = .1f);

    protected:
    virtual void Create() override;

    private:
    Curve* _profileCurve;
    Curve* _widthCurve;
    Curve* _compressedProfile;
    float _scale, _widthPos;
    int _resolutionVer = 50;
};


#endif // EXTRUDERFACE_H
