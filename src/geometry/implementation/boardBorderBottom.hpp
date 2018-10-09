#ifndef BOARDBORDERBOTTOM_H
#define BOARDBORDERBOTTOM_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "geometry/geometry.hpp"

class BoardBorderBottom : public Geometry{
    public:
    BoardBorderBottom();

    protected:
    virtual void Create() override;

    private:
};


#endif // BOARDBORDERBOTTOM_H
