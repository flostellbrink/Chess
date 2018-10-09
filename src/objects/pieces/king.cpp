#include <GL/glew.h>

#include "king.hpp"
#include <iostream>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "math.h"

#include "src/objects/objectManager.hpp"
#include "src/objects/moves/move.hpp"
#include "src/objects/moves/hit.hpp"
#include "src/objects/moves/rochade.hpp"

using glm::vec3;
using glm::mat4;

King::King(Board* board, int objectID, Field *field) : Piece(board, objectID, field) {

}

void King::init(){
    Piece::init();
    _yRotation = glm::pi<float>() / 2.f;
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
    if(_board->isRochadePossible(isWhite(), true))
        result.push_back(new Rochade(this, _board->GetRook(isWhite(), true), _field->Left->Left, _field->Left));
    if(_board->isRochadePossible(isWhite(), false))
        result.push_back(new Rochade(this, _board->GetRook(isWhite(), false), _field->Right->Right, _field->Right));
    return result;
}

bool King::isCopyable(){
    return false;
}
