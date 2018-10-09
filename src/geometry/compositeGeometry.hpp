#ifndef COMPOSITEGEOMETRY_H
#define COMPOSITEGEOMETRY_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/geometry/geometry.hpp"
#include "vector"

using std::vector;

class CompositeGeometry : public Geometry{
    public:
    CompositeGeometry(vector<Geometry*> geos);
    CompositeGeometry(Geometry* first, Geometry* second);

    virtual void Recreate() override;
    virtual void Draw() override;

    protected:
    virtual void Create() override;

    private:
    vector<Geometry*> _geos;
};


#endif // COMPOSITEGEOMETRY_H
