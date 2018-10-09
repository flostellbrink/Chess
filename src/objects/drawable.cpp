#include <GL/glew.h>
#include <iostream>

#include "drawable.hpp"
#include "src/texture/image.hpp"
#include "src/objects/objectManager.hpp"

vec3 Drawable::_lightPos = vec3(0,20,0);
vec3 Drawable::_camPos = vec3();
GLuint Drawable::ReflectionTexture;
GLuint Drawable::PostTexture;
GLuint Drawable::ShadowTexture;
mat4 Drawable::ShadowViewProjection;
int Drawable::OverlayState = 0;
float Drawable::OverlayOpacity = 1.f;

Drawable::Drawable(int objectId):
    _vertexArrayObject(0), _modelViewMatrix(1.0f), _objectId(objectId) {
}

void Drawable::init()
{
    initShader();
}

void Drawable::initShader()
{
    _program = new Shader(getVertexShader(), getFragmentShader());
    _programShadow = new Shader("res/shader/shadow.vs.glsl", "res/shader/shadow.fs.glsl");
}

void Drawable::drawOpaque(glm::mat4 projection_matrix){
    if(!ObjectManager::Textures.IsTranslucent(_objectId))
        draw(projection_matrix);
}

void Drawable::drawTranslucent(glm::mat4 projection_matrix){
    if(ObjectManager::Textures.IsTranslucent(_objectId))
        draw(projection_matrix);
}

void Drawable::drawReflection(glm::mat4 projection_matrix){
    // Ignores if translucent or not, because only translucent or only opaque objects are drawn
    if(ObjectManager::Textures.IsReflected(_objectId))
        draw(projection_matrix);
}
