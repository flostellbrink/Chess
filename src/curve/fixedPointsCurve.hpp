#ifndef FIXEDPOINTSCURVE_H
#define FIXEDPOINTSCURVE_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "curve.hpp"

class FixedPointsCurve : public Curve{
    public:
    FixedPointsCurve(ControlPoints controlPoints, int curveID, int objectID);
    virtual vector<vec2> InterpolatedPoints(int resolution = 10) override;
    virtual vec2 InterpolatedPoint(float value) override;
    virtual vector<vec2> InterpolatedTangents() override;
    virtual vec2 InterpolatedTangent(float value) override;

    protected:
    ControlPoints _controlPoints;
    int _objectID, _curveID;

    private:
};


#endif // FIXEDPOINTSCURVE_H
