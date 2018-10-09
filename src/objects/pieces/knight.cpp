#include <GL/glew.h>

#include "knight.hpp"
#include <iostream>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "math.h"

#include "src/objects/objectManager.hpp"
#include "src/objects/moves/move.hpp"
#include "src/objects/moves/hit.hpp"

using glm::vec3;
using glm::mat4;

Knight::Knight(Board* board, int objectID, Field *field) : Piece(board, objectID, field) {

}

void Knight::init(){
    Piece::init();
    if(!isWhite())
        _yRotation = glm::pi<float>();
}

vector<MoveBase*> Knight::GetMoves(){
    vector<MoveBase*> result;
    Field* current;
    if(_field->Up && (current = _field->Up->Up)){
        AddHitOrMove(current->Left, result);
        AddHitOrMove(current->Right, result);
    }
    if(_field->Down && (current = _field->Down->Down)){
        AddHitOrMove(current->Left, result);
        AddHitOrMove(current->Right, result);
    }
    if(_field->Left && (current = _field->Left->Left)){
        AddHitOrMove(current->Up, result);
        AddHitOrMove(current->Down, result);
    }
    if(_field->Right && (current = _field->Right->Right)){
        AddHitOrMove(current->Up, result);
        AddHitOrMove(current->Down, result);
    }
    return result;
}
