#include <GL/glew.h>

#include "bishop.hpp"
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

using glm::vec3;
using glm::mat4;

Bishop::Bishop(Board* board, int objectID, Field *field) : Piece(board, objectID, field) {

}

vector<MoveBase*> Bishop::GetMoves(){
    vector<MoveBase*> result;
    Field *currentField = _field;
    while(currentField->Up && (currentField = currentField->Up->Right)){
        AddHitOrMove(currentField, result);
        if(currentField->Piece)
            break;
    }
    currentField = _field;
    while(currentField->Down && (currentField = currentField->Down->Right)){
        AddHitOrMove(currentField, result);
        if(currentField->Piece)
            break;
    }
    currentField = _field;
    while(currentField->Up && (currentField = currentField->Up->Left)){
        AddHitOrMove(currentField, result);
        if(currentField->Piece)
            break;
    }
    currentField = _field;
    while(currentField->Down && (currentField = currentField->Down->Left)){
        AddHitOrMove(currentField, result);
        if(currentField->Piece)
            break;
    }
    return result;
}
