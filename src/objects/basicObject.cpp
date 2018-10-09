#include <GL/glew.h>

#include "basicObject.hpp"
#include <iostream>
#include "src/config.h"
#include "src/logger.h"

#include "math.h"

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
    delete _geo;
    _geo = ObjectManager::Geos.GetGeometry(_objectId);
}

vec3 BasicObject::Position(){
    return _position;
}

void BasicObject::Position(vec3 position){
    ObjectManager::Animations.PlayIndependent(new FadeAnimation<vec3>(500, _position, _position, position));
}

void BasicObject::draw(glm::mat4 projection_matrix){
    if(_program == nullptr){
        Logger::error("program not loaded");
    }
    _program->use();

    ObjectManager::Textures.Texture(_objectId)[0]->Bind(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, ReflectionTexture);
    glActiveTexture(GL_TEXTURE0 + 3);
    glBindTexture(GL_TEXTURE_2D, ShadowTexture);

    _program->bind(0, "tex");
    if (_program->hasUniform("texShadow")) {
        _program->bind(3, "texShadow");
    }

    _program->bind(projection_matrix, "view_projection_matrix");
    if (_program->hasUniform("view_projection_shadow")) {
        auto view_projection_shadow = ShadowViewProjection * _modelViewMatrix;
        _program->bind(view_projection_shadow, "view_projection_shadow");
    }

    _program->bind(_modelViewMatrix, "model_matrix");
    if (_program->hasUniform("tra_inv_model_matrix")) {
        auto tra_inv_model_matrix = glm::transpose(glm::inverse(_modelViewMatrix));
        _program->bind(tra_inv_model_matrix, "tra_inv_model_matrix");
    }

    if (_program->hasUniform("lightPos")) {
        _program->bind(_lightPos, "lightPos");
    }
    if (_program->hasUniform("camPos")) {
        _program->bind(_camPos, "camPos");
    }

    if (_program->hasUniform("La")) {
        auto La = vec3(.5, .5, .5);
        _program->bind(La, "La");
    }
    if (_program->hasUniform("ka")) {
        auto ka = vec3(.5, .5, .5);
        _program->bind(ka, "ka");
    }
    if (_program->hasUniform("Ld")) {
        auto Ld = vec3(.5, .5, .5);
        _program->bind(Ld, "Ld");
    }
    if (_program->hasUniform("kd")) {
        auto kd = vec3(1, 1, 1);
        _program->bind(kd, "kd");
    }

    if (_program->hasUniform("reflectivity")) {
        _program->bind(ObjectManager::Textures.reflectivity(_objectId), "reflectivity");
    }
    if (_program->hasUniform("shininess")) {
        _program->bind(ObjectManager::Textures.shininess(_objectId), "shininess");
    }

    _geo->Draw();
}

void BasicObject::drawShadow(glm::mat4 projection_matrix){
    _programShadow->use();

    auto view_projection_shadow = projection_matrix * _modelViewMatrix;
    _programShadow->bind(view_projection_shadow, "view_projection_shadow");

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
    return "res/shader/" + _shader + ".vs.glsl";
}

std::string BasicObject::getFragmentShader()
{
    return "res/shader/" + _shader + ".fs.glsl";
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
