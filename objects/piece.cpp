#include <GL/glew.h>

#include "piece.hpp"
#include <iostream>
#include "gltool.hpp"
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include "math.h"
#ifndef M_PI
#define M_PI glm::pi<float>()
#endif

#include "objectManager.hpp"
#include "animation/fadeAnimation.hpp"
#include "animation/backLoopingAnimation.hpp"
#include "animation/catmullRomAnimation.hpp"
#include "moves/hit.hpp"
#include "moves/move.hpp"

using glm::value_ptr;
using glm::vec3;
using glm::mat4;

Piece::Piece(Board* board, int objectID, Field* field)
    : Drawable(objectID), _field(field), _position(field->TopPosition()), _yRotation(0) {
    _field->CurrentPiece = this;
    _board = board;
    BoundingBox = ObjectManager::Instance.Collisions.GetAABB(vec3(), vec3());
    UpdateBB(Position());
}

void Piece::init(){
    Drawable::init();
    recreateGeoemtry();
}

void Piece::recreateGeoemtry(){
    if(_geo) {
        delete _geo;
    }
    _geo = ObjectManager::Geos.GetGeometry(_objectId);
}

void Piece::UpdateBB(vec3 position){
    vec3 size = vec3(1.5f, 5, 1.5f);
    BoundingBox->min = position - size * .5f;
    BoundingBox->max = position + size * .5f;
}

vec3 Piece::Position(){
    return _position;
}

Field* Piece::GetField(){
    return _field;
}

void Piece::SetField(Field* field, bool sim){
    if(field == _field) {
        _field->CurrentPiece = this;
        return;
    }
    if(_field->CurrentPiece == this)
        _field->CurrentPiece = 0;
    if(!sim){
        //TODO check a few collision cases
        // _______
        // Is straight line free? Fade Animation
        // Is straight line with smaller boxes possible?
        // Move other pieces a little to the side (affected pieces are next to the diagonal) then fade then move others back
        // _______
        //        |
        // Is straight line from start to end, with ends height set to starts heigt possible?
        // Catmull rom: start -> heightend end -> end
        //   ____
        // _/    \_
        //   _____
        // _/     |
        //  _____
        // |     \_
        // Is slightly diagonal up possible?
        //  ______
        // |      |
        // else Catmull rom: start -> start(height +) -> end(height of start+) -> end

        vec3 from = _field->TopPosition(), to = field->TopPosition(), direction = to - from, up = vec3(0,1,0);
        float duration = 1000;
        UpdateBB(to);

        Collision* directLine = ObjectManager::Instance.Collisions.GetRay(from + up * .1f, to + up * .1f);
        if(!_board->intersectsGame(directLine, this)){
            ObjectManager::Animations.PlayLast(new FadeAnimation<vec3>(duration, _position, from, to));
        } else {
            float maxY = std::max(from.y, to.y);
            vec3 p1 = from + direction * .1f, p2 = to - direction * .1f;
            p1.y = p2.y = maxY + 3;
            ObjectManager::Animations.PlayLast(new CatmullRomAnimation<vec3>(duration, _position,
                vector<vec3>{from, p1, p2, to}));
        }
    }
    _field = field;
    _field->CurrentPiece = this;
}

bool Piece::isWhite(){
    return !(_objectId % 2);
}

bool Piece::isTransformable(){
    return false;
}

bool Piece::isCopyable(){
    return true;
}

int Piece::GetIdWithoutColor(){
    return _objectId - 1 + isWhite();
}

void Piece::draw(glm::mat4 projection_matrix){
    if(_program == 0){
        std::cerr << "program not loaded" << std::endl;
    }
    glUseProgram(_program);

    vector<TextureWrapper*> textures = ObjectManager::Textures.Texture(_objectId);
    if(textures.size() > 0)
        textures[0]->Bind();

    glActiveTexture(GL_TEXTURE0 + 3);
    glBindTexture(GL_TEXTURE_2D, ShadowTexture);

    glUniform1i(glGetUniformLocation(_program, "tex"), 0);
    glUniform1i(glGetUniformLocation(_program, "texShadow"), 3);

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

void Piece::drawShadow(glm::mat4 projection_matrix){
    glUseProgram(_programShadow);
    glUniformMatrix4fv(glGetUniformLocation(_programShadow, "view_projection_shadow"), 1, GL_FALSE, glm::value_ptr(projection_matrix * _modelViewMatrix));
    _geo->Draw();
}

void Piece::update(float elapsedTimeMs){
    // Ignore parameter unused
    (void)elapsedTimeMs;

    _modelViewMatrix = glm::translate(mat4(), Position());
    _modelViewMatrix = glm::rotate(_modelViewMatrix, _yRotation, vec3(0,1,0));
}

void Piece::MouseClick(glm::vec3 position){
    // handled by field
    // Ignore parameter unused
    (void)position;
}

void Piece::AddHitOrMove(Field *field, vector<MoveBase *> &moves){
    if(!field) return;
    if(!field->CurrentPiece)
        moves.push_back(new Move(this, field));
    else if(field->CurrentPiece->isWhite() != isWhite())
        moves.push_back(new Hit(this, field->CurrentPiece));
}

std::string Piece::getVertexShader()
{
    return Drawable::loadShaderFile(":/shader/basic.vs.glsl");
}

std::string Piece::getFragmentShader()
{
    return Drawable::loadShaderFile(":/shader/basic.fs.glsl");
}

vec3 Piece::Position3D(){
    return Position();
}
