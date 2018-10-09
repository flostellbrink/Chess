#include <GL/glew.h>

#include "field.hpp"

#include "glbase/gltool.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "objectManager.hpp"
#include "objects.hpp"
#include "animation/loopingAnimation.hpp"
#include "animation/backLoopingAnimation.hpp"
#include "animation/fadeAnimation.hpp"
#include "config.h"

#include <iostream>

using glm::mat2;
using glm::vec3;
using glm::mat4;
using glm::value_ptr;


float Field::overlayRotation = 0, Field::overlayScale = 1;
Field::constructor Field::cons;

Field::constructor::constructor(){
    ObjectManager::Instance.Animations.PlayIndependent(new BackLoopingAnimation<float>(new FadeAnimation<float>(500, overlayScale, 1, 1.2)));
    ObjectManager::Instance.Animations.PlayIndependent(new LoopingAnimation<float>(new LinearAnimation<float>(5000, overlayRotation, 0, 2 * M_PI)));
}

Field::Field(Board* board, int boardX, int boardY, bool inactive):
    Drawable() {
    _inactive = inactive;
    _board = board;
    _boardX = boardX;
    _boardY = boardY;
    _position = vec3(_boardX * 2 - 7, 0, _boardY * 2 - 7);
    _objectId = objects::Field00 + boardX * 8 + boardY;
    vec3 size = vec3(2,10,2);
    _boundingBox = ObjectManager::Collisions.GetAABB(CenterPosition() - size * .5f, CenterPosition() + size * .5f);
}

void Field::init(){
    Drawable::init();
    _geo = ObjectManager::Geos.GetCube();

    _modelViewMatrix = glm::scale(mat4(), vec3(1,.5,1));
    _modelViewMatrix = glm::translate(_modelViewMatrix, CenterPosition());
}

void Field::recreateGeoemtry(){
    // Fields geometry is always a cube
}

void Field::draw(glm::mat4 projection_matrix){
    if(_inactive) return;

    if(_program == 0){
        std::cerr << "program not loaded" << std::endl;
    }
    glUseProgram(_program);

    ObjectManager::Textures.Texture(_objectId)[0]->Bind();
    if(_ovelayOpacity > 0){
        ObjectManager::Textures.Texture(_objectId)[_overlayNumber]->Bind(GL_TEXTURE_2D, 1);
        bool animateOverlay = _overlayNumber == 2;
        glUniform1i(glGetUniformLocation(_program, "textureAnimated"), animateOverlay);
        if(animateOverlay){
            mat2 rotation = mat2(cos(overlayRotation), sin(overlayRotation), -sin(overlayRotation), cos(overlayRotation));
            rotation *= overlayScale;
            glUniformMatrix2fv(glGetUniformLocation(_program, "texTransform"), 1, GL_FALSE, glm::value_ptr(rotation));
        }
    }

    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, ReflectionTexture);
    glActiveTexture(GL_TEXTURE0 + 3);
    glBindTexture(GL_TEXTURE_2D, ShadowTexture);

    glUniform1i(glGetUniformLocation(_program, "tex"), 0);
    glUniform1i(glGetUniformLocation(_program, "texOverlay"), 1);
    glUniform1i(glGetUniformLocation(_program, "texReflection"), 2);
    glUniform1i(glGetUniformLocation(_program, "texShadow"), 3);

    glUniform2fv(glGetUniformLocation(_program, "texSize"), 1, value_ptr(vec2(Config::viewportWidth, Config::viewportHeight)));

    glUniform1f(glGetUniformLocation(_program, "overlayOpacity"), _ovelayOpacity);

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

void Field::drawShadow(glm::mat4 projection_matrix){
    if(_inactive) return;

    glUseProgram(_programShadow);
    glUniformMatrix4fv(glGetUniformLocation(_programShadow, "view_projection_shadow"), 1, GL_FALSE, glm::value_ptr(projection_matrix * _modelViewMatrix));
    _geo->Draw();
}

void Field::update(float elapsedTimeMs){
    // Ignore parameter unused
    (void)elapsedTimeMs;
}

void Field::MouseClick(glm::vec3 position){
    if(!_boundingBox->Contains(position)) return;
    if(CurrentPiece)
        _board->PieceClick(CurrentPiece);
    else
        _board->FieldClick(this);
}

vec3 Field::TopPosition(){
    if(_inactive)
        return CenterPosition();
    return _position;
}

vec3 Field::CenterPosition(){
    return _position - vec3(0,1,0);
}

std::string Field::getVertexShader(){
    return Drawable::loadShaderFile(":/shader/mirror.vs.glsl");
}

std::string Field::getFragmentShader(){
    return Drawable::loadShaderFile(":/shader/mirror.fs.glsl");
}

void Field::EnableOverlay(bool enabled){
    if(enabled != _overlayEnabled)
        ObjectManager::Animations.PlayIndependent(new FadeAnimation<float>(350, _ovelayOpacity, _overlayEnabled, enabled));
    _overlayEnabled = enabled;
}

void Field::SetOverlayNumber(int overlay){
    _overlayNumber = overlay;
}

int Field::Row(){
    return _boardX;
}

int Field::Column(){
    return _boardY;
}

vec3 Field::Position3D(){
    return CenterPosition();
}
