#ifndef FIXEDPOINTSCURVE_H
#define FIXEDPOINTSCURVE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "curve.hpp"

class FixedPointsCurve : public Curve{
    public:
    FixedPointsCurve(ControlPoints controlPoints, int curveID, int objectID);
    vector<vec2> InterpolatedPoints(int resolution) override;
    vec2 InterpolatedPoint(float value) override;
    vector<vec2> InterpolatedTangents() override;
    vec2 InterpolatedTangent(float value) override;

    protected:
    ControlPoints _controlPoints;
    int _objectID, _curveID;

    private:
};


#endif // FIXEDPOINTSCURVE_H
