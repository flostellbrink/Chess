#ifndef CURVE_H
#define CURVE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/common.hpp>
#include "src/geometry/controlPoints.hpp"

using std::vector;
using glm::vec2;

class Curve{
    public:
    Curve();
    /**
     * @brief InterpolatedPoints generates points between control points
     * @param resolution steps between control points
     * @return vector of points on the curve. result size may vary
     */
    virtual vector<vec2> InterpolatedPoints(int resolution = 10) = 0;

    /**
     * @brief InterpolatedPoints calculates an interpolated point
     * @param value position between 0 and 1 on the curve
     * @return interpolated point
     */
    virtual vec2 InterpolatedPoint(float value) = 0;

    /**
     * @brief InterpolatedTangents calculates tangents between control points, takes resolution from last InterpolatedPoints call
     * @return vector of tangents on the curve. result size may vary
     */
    virtual vector<vec2> InterpolatedTangents() = 0;

    /**
     * @brief InterpolatedTangent calculates an interpolted tangent
     * @param value position between 0 and 1 on the curve
     * @return interpolates tangent
     */
    virtual vec2 InterpolatedTangent(float value) = 0;

    protected:
    vector<vec2> _tangents;

    private:
};


#endif // CURVE_H
