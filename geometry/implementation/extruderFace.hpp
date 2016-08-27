#ifndef EXTRUDERFACE_H
#define EXTRUDERFACE_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "geometry/geometry.hpp"
#include "curve/curve.hpp"

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
