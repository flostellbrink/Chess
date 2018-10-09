// Loads geometry for all objects
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <vector>

using std::vector;
using glm::vec2;
using glm::vec3;

class Geometry{
    public:
    Geometry();
    virtual ~Geometry();
    virtual void Recreate();
    virtual void Draw();

    protected:
    virtual void Create() = 0;

    GLuint _vertexArrayObject;  /**< The vertex array object containing the vertices */

    vector<vec3> _geoPositions;
    vector<vec3> _geoNormals;
    vector<unsigned int> _geoIndices;
    vector<vec2> _geoTextureCoords;

    private:
    void BeforeCreate();
    void FinishCreate();
};


#endif // GEOMETRY_H
