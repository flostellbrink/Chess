#ifndef BOARDBORDERBOTTOM_H
#define BOARDBORDERBOTTOM_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/geometry/geometry.hpp"

class BoardBorderBottom : public Geometry{
    public:
    BoardBorderBottom();

    protected:
    virtual void Create() override;

    private:
};


#endif // BOARDBORDERBOTTOM_H
