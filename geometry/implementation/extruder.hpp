#ifndef EXTRUDER_H
#define EXTRUDER_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "geometry/geometry.hpp"
#include "curve/curve.hpp"

class Extruder : public Geometry{
    public:
    Extruder(Curve* profileCurve, Curve* widthCurve, float scale = .1f);

    protected:
    virtual void Create() override;

    private:
    Curve* _profileCurve;
    Curve* _widthCurve;
    float _scale;
    int _resolution = 10;
};


#endif // EXTRUDER_H
