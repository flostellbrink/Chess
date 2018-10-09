#ifndef BOARDBORDERTOP_H
#define BOARDBORDERTOP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/geometry/geometry.hpp"

class BoardBorderTop : public Geometry{
    public:
    BoardBorderTop();

    protected:
    virtual void Create() override;

    private:
};


#endif // BOARDBORDERTOP_H
