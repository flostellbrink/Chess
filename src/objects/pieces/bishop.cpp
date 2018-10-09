#include <GL/glew.h>

#include "bishop.hpp"
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

Bishop::Bishop(Board* board, int objectID, Field *field) : Piece(board, objectID, field) {

}

vector<MoveBase*> Bishop::GetMoves(){
    vector<MoveBase*> result;
    Field *currentField = _field;
    while(currentField->Up && (currentField = currentField->Up->Right)){
        AddHitOrMove(currentField, result);
        if(currentField->CurrentPiece)
            break;
    }
    currentField = _field;
    while(currentField->Down && (currentField = currentField->Down->Right)){
        AddHitOrMove(currentField, result);
        if(currentField->CurrentPiece)
            break;
    }
    currentField = _field;
    while(currentField->Up && (currentField = currentField->Up->Left)){
        AddHitOrMove(currentField, result);
        if(currentField->CurrentPiece)
            break;
    }
    currentField = _field;
    while(currentField->Down && (currentField = currentField->Down->Left)){
        AddHitOrMove(currentField, result);
        if(currentField->CurrentPiece)
            break;
    }
    return result;
}
