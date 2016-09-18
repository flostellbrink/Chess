#include <GL/glew.h>

#include "basicObject.hpp"
#include <iostream>
#include "gui/config.h"

#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include "math.h"
#ifndef M_PI
#define M_PI glm::pi<float>()
#endif

using glm::value_ptr;
using glm::vec3;
using glm::mat4;

BasicObject::BasicObject(int objectID, vec3 position, float yRotation, string shader)
    :Drawable(objectID), _position(position), _yRotation(yRotation), _shader(shader){

}

void BasicObject::init(){
    Drawable::init();
    recreateGeoemtry();
}

void BasicObject::recreateGeoemtry(){
    if(_geo) {
        delete _geo;
    }
    _geo = ObjectManager::Geos.GetGeometry(_objectId);
}

vec3 BasicObject::Position(){
    return _position;
}

void BasicObject::Position(vec3 position){
    ObjectManager::Instance.Animations.PlayIndependent(new FadeAnimation<vec3>(500, _position, _position, position));
}

void BasicObject::draw(glm::mat4 projection_matrix){
    if(_program == 0){
        std::cerr << "program not loaded" << std::endl;
    }
    glUseProgram(_program);

    ObjectManager::Textures.Texture(_objectId)[0]->Bind(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, ReflectionTexture);
    glActiveTexture(GL_TEXTURE0 + 3);
    glBindTexture(GL_TEXTURE_2D, ShadowTexture);

    glUniform1i(glGetUniformLocation(_program, "tex"), 0);
    glUniform1i(glGetUniformLocation(_program, "texReflection"), 2);
    glUniform1i(glGetUniformLocation(_program, "texShadow"), 3);

    glUniform2fv(glGetUniformLocation(_program, "texSize"), 1, value_ptr(vec2(Config::viewportWidth, Config::viewportHeight)));

    glUniformMatrix4fv(glGetUniformLocation(_program, "view_projection_matrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(glGetUniformLocation(_program, "view_projection_shadow"), 1, GL_FALSE, glm::value_ptr(ShadowViewProjection * _modelViewMatrix));

    glUniformMatrix4fv(glGetUniformLocation(_program, "model_matrix"), 1, GL_FALSE, glm::value_ptr(_modelViewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(_program, "tra_inv_model_matrix"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(_modelViewMatrix))));

    glUniform3fv(glGetUniformLocation(_program, "lightPos"), 1, value_ptr(_lightPos));
    glUniform3fv(glGetUniformLocation(_program, "camPos"), 1, value_ptr(_camPos));

    vec3 La = vec3(.5, .5, .5);
    glUniform3fv(glGetUniformLocation(_program, "La"), 1, value_ptr(La));
    vec3 ka = vec3(.5, .5, .5);
    glUniform3fv(glGetUniformLocation(_program, "ka"), 1, value_ptr(ka));
    vec3 Ld = vec3(.5, .5, .5);
    glUniform3fv(glGetUniformLocation(_program, "Ld"), 1, value_ptr(Ld));
    vec3 kd = vec3(1, 1, 1);
    glUniform3fv(glGetUniformLocation(_program, "kd"), 1, value_ptr(kd));

    glUniform1f(glGetUniformLocation(_program, "reflectivity"), ObjectManager::Textures.reflectivity(_objectId));
    glUniform1f(glGetUniformLocation(_program, "shininess"), ObjectManager::Textures.shininess(_objectId));

    _geo->Draw();
}

void BasicObject::drawShadow(glm::mat4 projection_matrix){
    glUseProgram(_programShadow);
    glUniformMatrix4fv(glGetUniformLocation(_programShadow, "view_projection_shadow"), 1, GL_FALSE, glm::value_ptr(projection_matrix * _modelViewMatrix));
    _geo->Draw();
}

void BasicObject::update(float elapsedTimeMs){
    // Ignore parameter unused
    (void)elapsedTimeMs;

    _modelViewMatrix = glm::translate(mat4(), Position());
    _modelViewMatrix = glm::rotate(_modelViewMatrix, _yRotation, vec3(0,1,0));
    _modelViewMatrix = glm::rotate(_modelViewMatrix, _xRotation, vec3(1,0,0));
    _modelViewMatrix = glm::rotate(_modelViewMatrix, _zRotation, vec3(0,0,1));
}

void BasicObject::MouseClick(glm::vec3 position){
    // Ignore parameter unused
    (void)position;
}

std::string BasicObject::getVertexShader()
{
    return Drawable::loadShaderFile(":/shader/" + _shader + ".vs.glsl");
}

std::string BasicObject::getFragmentShader()
{
    return Drawable::loadShaderFile(":/shader/" + _shader + ".fs.glsl");
}

vec3 BasicObject::Position3D(){
    return Position();
}

void BasicObject::setRotationX(float val) {
    _xRotation = val;
}

void BasicObject::setRotationY(float val) {
    _yRotation = val;
}

void BasicObject::setRotationZ(float val) {
    _zRotation = val;
}
