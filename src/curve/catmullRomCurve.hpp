#ifndef CATMULLROMCURVE_H
#define CATMULLROMCURVE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "curve.hpp"

class CatmullRomCurve : public Curve{
    public:
    CatmullRomCurve(ControlPoints controlPoints, int curveID, int objectID);
    virtual vector<vec2> InterpolatedPoints(int resolution = 10) override;
    virtual vec2 InterpolatedPoint(float value) override;
    virtual vector<vec2> InterpolatedTangents() override;
    virtual vec2 InterpolatedTangent(float value) override;

    protected:
    ControlPoints _controlPoints;
    int _objectID, _curveID;

    private:
    int _lastResolution;
};


#endif // CATMULLROMCURVE_H
