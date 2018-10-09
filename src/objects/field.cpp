#include <GL/glew.h>
#include "field.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "objectManager.hpp"
#include "src/objects.hpp"
#include "src/animation/loopingAnimation.hpp"
#include "src/animation/backLoopingAnimation.hpp"
#include "src/animation/fadeAnimation.hpp"
#include "src/config.h"

#include <iostream>
#include <src/logger.h>

using glm::mat2;
using glm::vec3;
using glm::mat4;
using glm::value_ptr;


float Field::overlayRotation = 0, Field::overlayScale = 1;
Field::constructor Field::cons;

Field::constructor::constructor(){
    ObjectManager::Instance.Animations.PlayIndependent(new BackLoopingAnimation<float>(new FadeAnimation<float>(500, overlayScale, 1, 1.2)));
    ObjectManager::Instance.Animations.PlayIndependent(new LoopingAnimation<float>(new LinearAnimation<float>(5000, overlayRotation, 0, 2 * glm::pi<float>())));
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

    if(_program == nullptr){
        Logger::error("program not loaded");
    }
    _program->use();

    ObjectManager::Textures.Texture(_objectId)[0]->Bind();
    if(_ovelayOpacity > 0){
        ObjectManager::Textures.Texture(_objectId)[_overlayNumber]->Bind(GL_TEXTURE_2D, 1);
        bool animateOverlay = _overlayNumber == 2;
        _program->bind(animateOverlay, "textureAnimated");
        if(animateOverlay){
            mat2 rotation = mat2(cos(overlayRotation), sin(overlayRotation), -sin(overlayRotation), cos(overlayRotation));
            rotation *= overlayScale;
            _program->bind(rotation, "texTransform");
        }
    }

    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, ReflectionTexture);
    glActiveTexture(GL_TEXTURE0 + 3);
    glBindTexture(GL_TEXTURE_2D, ShadowTexture);

    _program->bind(0, "tex");
    _program->bind(1, "texOverlay");
    _program->bind(2, "texReflection");
    _program->bind(3, "texShadow");

    auto texSize = vec2(Config::viewportWidth, Config::viewportHeight);
    _program->bind(texSize, "texSize");

    _program->bind(_ovelayOpacity, "overlayOpacity");

    auto view_projection_shadow = ShadowViewProjection * _modelViewMatrix;
    auto tra_inv_model_matrix = glm::transpose(glm::inverse(_modelViewMatrix));
    _program->bind(projection_matrix, "view_projection_matrix");
    _program->bind(view_projection_shadow, "view_projection_shadow");
    _program->bind(_modelViewMatrix, "model_matrix");
    _program->bind(tra_inv_model_matrix, "tra_inv_model_matrix");

    _program->bind(_lightPos, "lightPos");
    _program->bind(_camPos, "camPos");

    vec3 La = vec3(.5, .5, .5);
    vec3 ka = vec3(.5, .5, .5);
    vec3 Ld = vec3(.5, .5, .5);
    vec3 kd = vec3(1, 1, 1);
    _program->bind(La, "La");
    _program->bind(ka, "ka");
    _program->bind(Ld, "Ld");
    _program->bind(kd, "kd");

    _program->bind(ObjectManager::Textures.reflectivity(_objectId), "reflectivity");
    _program->bind(ObjectManager::Textures.shininess(_objectId), "shininess");

    _geo->Draw();
}

void Field::drawShadow(glm::mat4 projection_matrix){
    if(_inactive) return;
    _programShadow->use();

    auto view_projection_shadow = projection_matrix * _modelViewMatrix;
    _programShadow->bind(view_projection_shadow, "view_projection_shadow");

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
    return "res/shader/mirror.vs.glsl";
}

std::string Field::getFragmentShader(){
    return "res/shader/mirror.fs.glsl";
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
