#ifndef QUAD_H
#define QUAD_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "geometry/geometry.hpp"

class Quad : public Geometry{
    public:
    Quad();

    protected:
    virtual void Create() override;

    private:
};


#endif // QUAD_H
