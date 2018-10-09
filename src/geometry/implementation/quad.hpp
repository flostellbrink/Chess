#ifndef QUAD_H
#define QUAD_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/geometry/geometry.hpp"

class Quad : public Geometry{
    public:
    Quad();

    protected:
    virtual void Create() override;

    private:
};


#endif // QUAD_H
