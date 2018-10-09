#ifndef REVOLVER_H
#define REVOLVER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/geometry/geometry.hpp"
#include "src/curve/curve.hpp"

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
