#ifndef CUBE_H
#define CUBE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/geometry/geometry.hpp"

class Cube : public Geometry{
    public:
    Cube();

    protected:
    virtual void Create() override;

    private:
};


#endif // CUBE_H
