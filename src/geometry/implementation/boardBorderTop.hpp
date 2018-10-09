#ifndef BOARDBORDERTOP_H
#define BOARDBORDERTOP_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "geometry/geometry.hpp"

class BoardBorderTop : public Geometry{
    public:
    BoardBorderTop();

    protected:
    virtual void Create() override;

    private:
};


#endif // BOARDBORDERTOP_H
