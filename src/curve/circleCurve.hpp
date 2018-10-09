#ifndef CIRCLECURVE_H
#define CIRCLECURVE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "curve.hpp"

class CircleCurve : public Curve{
    public:
    CircleCurve(float radius = 1.f);
    virtual vector<vec2> InterpolatedPoints(int resolution = 10) override;
    virtual vec2 InterpolatedPoint(float value) override;
    virtual vector<vec2> InterpolatedTangents() override;
    virtual vec2 InterpolatedTangent(float value) override;

    protected:
    float _radius;

    private:
};


#endif // CIRCLECURVE_H
