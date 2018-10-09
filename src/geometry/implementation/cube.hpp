#ifndef CUBE_H
#define CUBE_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "geometry/geometry.hpp"

class Cube : public Geometry{
    public:
    Cube();

    protected:
    virtual void Create() override;

    private:
};


#endif // CUBE_H
