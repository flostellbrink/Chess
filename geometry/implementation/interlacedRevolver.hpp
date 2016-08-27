#ifndef INTERLACEDREVOLVER_H
#define INTERLACEDREVOLVER_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "geometry/geometry.hpp"
#include "curve/curve.hpp"

class InterlacedRevolver : public Geometry{
    public:
    /**
     * @brief InterlacedRevolver Used for the knight, toggles between two height profiles at given lenght values
     * @param baseCurve Curve for x and z axis
     * @param heightCurve1 First Curve for y axis
     * @param heightCurve2 Second Curve for y axis
     * @param length1 Size of First Curves Segments
     * @param length2 Size of Second Curves Segements
     */
    InterlacedRevolver(Curve* baseCurve, Curve* heightCurve1, Curve* heightCurve2, float length1, float length2, float scale = .1);

    protected:
    virtual void Create() override;

    private:
    Curve* _baseCurve;
    Curve* _heightCurve1;
    Curve* _heightCurve2;
    float _scale, _length1, _length2;
    int _resolutionHor = 50, _resolutionVer = 100;
};


#endif // INTERLACEDREVOLVER_H
