#ifndef REVOLVER_H
#define REVOLVER_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "geometry/geometry.hpp"
#include "curve/curve.hpp"

class Revolver : public Geometry{
    public:
    Revolver(Curve* baseCurve, Curve* heightCurve, float scale = .1, bool clockTexture = false);

    protected:
    virtual void Create() override;

    private:
    Curve* _baseCurve;
    Curve* _heightCurve;
    float _scale;
    int _resolutionHor = 50, _resolutionVer = 100;
    bool _clockTexture;
};


#endif // REVOLVER_H
