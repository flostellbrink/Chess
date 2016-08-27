#include <GL/glew.h>

#include "king.hpp"
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

#include "objects/objectManager.hpp"
#include "objects/moves/move.hpp"
#include "objects/moves/hit.hpp"
#include "objects/moves/rochade.hpp"

using glm::vec3;
using glm::mat4;

King::King(Board* board, int objectID, Field *field) : Piece(board, objectID, field) {

}

void King::init(){
    Piece::init();
    _yRotation = M_PI / 2.;
}

vector<MoveBase*> King::GetMoves(){
    vector<MoveBase*> result;
    AddHitOrMove(_field->Up, result);
    AddHitOrMove(_field->Down, result);
    AddHitOrMove(_field->Left, result);
    AddHitOrMove(_field->Right, result);
    if(_field->Up){
        AddHitOrMove(_field->Up->Left, result);
        AddHitOrMove(_field->Up->Right, result);
    }
    if(_field->Down){
        AddHitOrMove(_field->Down->Left, result);
        AddHitOrMove(_field->Down->Right, result);
    }
    if(_board->isRochadePossible(isWhite(), 1))
        result.push_back(new Rochade(this, _board->GetRook(isWhite(), 1), _field->Left->Left, _field->Left));
    if(_board->isRochadePossible(isWhite(), 0))
        result.push_back(new Rochade(this, _board->GetRook(isWhite(), 0), _field->Right->Right, _field->Right));
    return result;
}

bool King::isCopyable(){
    return false;
}
