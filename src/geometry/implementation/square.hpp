#ifndef SQUARE_H
#define SQUARE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/geometry/geometry.hpp"

class Square : public Geometry{
    public:
    Square();

    protected:
    virtual void Create() override;

    private:
};


#endif // SQUARE_H
