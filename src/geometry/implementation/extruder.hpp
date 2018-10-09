#ifndef EXTRUDER_H
#define EXTRUDER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/geometry/geometry.hpp"
#include "src/curve/curve.hpp"

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
