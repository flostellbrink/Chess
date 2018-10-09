#include <GL/glew.h>

#include "rook.hpp"
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

Rook::Rook(Board* board, int objectID, Field *field) : Piece(board, objectID, field) {

}

vector<MoveBase*> Rook::GetMoves(){
    vector<MoveBase*> result;
    Field *currentField = _field;
    while((currentField = currentField->Up)){
        AddHitOrMove(currentField, result);
        if(currentField->CurrentPiece)
            break;
    }
    currentField = _field;
    while((currentField = currentField->Down)){
        AddHitOrMove(currentField, result);
        if(currentField->CurrentPiece)
            break;
    }
    currentField = _field;
    while((currentField = currentField->Left)){
        AddHitOrMove(currentField, result);
        if(currentField->CurrentPiece)
            break;
    }
    currentField = _field;
    while((currentField = currentField->Right)){
        AddHitOrMove(currentField, result);
        if(currentField->CurrentPiece)
            break;
    }
    return result;
}
