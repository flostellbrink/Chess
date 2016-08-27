#ifndef SQUARE_H
#define SQUARE_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "geometry/geometry.hpp"

class Square : public Geometry{
    public:
    Square();

    protected:
    virtual void Create() override;

    private:
};


#endif // SQUARE_H
