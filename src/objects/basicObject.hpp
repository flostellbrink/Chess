#ifndef BASICOBJECT_H
#define BASICOBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "drawable.hpp"
#include "field.hpp"

class Field;

class BasicObject: public Drawable{
    public:
    BasicObject(int objectID, vec3 position = vec3(), float yRotation = 0, string shader = "basic");
    virtual void init() override;
    virtual void draw(glm::mat4 projection_matrix) override;
    virtual void drawShadow(mat4 projection_matrix) override;

    virtual void update(float elapsedTimeMs) override;
    virtual void MouseClick(vec3 position) override;
    vec3 Position();
    void Position(vec3 position);
    virtual vec3 Position3D() override;
    void setRotationX(float val);
    void setRotationY(float val);
    void setRotationZ(float val);

    void recreateGeoemtry() override;

    protected:
    virtual std::string getVertexShader() override;
    virtual std::string getFragmentShader() override;
    vec3 _position;
    float _yRotation;
    float _xRotation = 0.f;
    float _zRotation = 0.f;
    string _shader;

    Geometry* _geo = 0;
    Field* _field;

    private:
};


#endif // BASICOBJECT_H
