#include <GL/glew.h>

#include "pawn.hpp"
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

Pawn::Pawn(Board* board, int objectID, Field* field) : Piece(board, objectID, field) {

}

vector<MoveBase*> Pawn::GetMoves(){
    MoveBase* lastMove = _board->GetLastMove();
    vector<MoveBase*> result;
    if(isWhite()){
        if(_field->Up){
            if(!_field->Up->Piece)
                result.push_back(new Move(this, _field->Up));
            if(!UsedDoubleMove() && _field->Up->Up && !_field->Up->Up->Piece && !_field->Up->Piece){
                Move* move = new Move(this, _field->Up->Up);
                move->EnPassantField = _field->Up;
                move->IsEnPassant = true;
                result.push_back(move);
            }
            if(_field->Up->Right && _field->Up->Right->Piece && _field->Up->Right->Piece->isWhite() != isWhite())
                result.push_back(new Hit(this, _field->Up->Right->Piece));
            if(_field->Up->Left && _field->Up->Left->Piece && _field->Up->Left->Piece->isWhite() != isWhite())
                result.push_back(new Hit(this, _field->Up->Left->Piece));
            if(lastMove && lastMove->IsEnPassant && lastMove->EnPassantField && lastMove->MainPiece && lastMove->MainPiece->isWhite() != isWhite() && _field->Up->Right && !_field->Up->Right->Piece && lastMove->EnPassantField == _field->Up->Right)
                result.push_back(new Hit(this, lastMove->MainPiece, lastMove->EnPassantField));
            if(lastMove && lastMove->IsEnPassant && lastMove->EnPassantField && lastMove->MainPiece && lastMove->MainPiece->isWhite() != isWhite() && _field->Up->Left && !_field->Up->Left->Piece && lastMove->EnPassantField == _field->Up->Left)
                result.push_back(new Hit(this, lastMove->MainPiece, lastMove->EnPassantField));
        }
    } else {
        if(_field->Down){
            if(!_field->Down->Piece)
                result.push_back(new Move(this, _field->Down));
            if(!UsedDoubleMove() &&_field->Down->Down && !_field->Down->Down->Piece && !_field->Down->Piece){
                Move* move = new Move(this, _field->Down->Down);
                move->EnPassantField = _field->Down;
                move->IsEnPassant = true;
                result.push_back(move);
            }
            if(_field->Down->Right && _field->Down->Right->Piece && _field->Down->Right->Piece->isWhite() != isWhite())
                result.push_back(new Hit(this, _field->Down->Right->Piece));
            if(_field->Down->Left && _field->Down->Left->Piece && _field->Down->Left->Piece->isWhite() != isWhite())
                result.push_back(new Hit(this, _field->Down->Left->Piece));
            if(lastMove && lastMove->IsEnPassant && lastMove->EnPassantField && lastMove->MainPiece && lastMove->MainPiece->isWhite() != isWhite() && _field->Down->Right && !_field->Down->Right->Piece && lastMove->EnPassantField == _field->Down->Right)
                result.push_back(new Hit(this, lastMove->MainPiece, lastMove->EnPassantField));
            if(lastMove && lastMove->IsEnPassant && lastMove->EnPassantField && lastMove->MainPiece && lastMove->MainPiece->isWhite() != isWhite() && _field->Down->Left && !_field->Down->Left->Piece && lastMove->EnPassantField == _field->Down->Left)
                result.push_back(new Hit(this, lastMove->MainPiece, lastMove->EnPassantField));
        }
    }
    return result;
}

bool Pawn::UsedDoubleMove(){
    int startRow = isWhite() ? 1 : 6;
    return _field->Row() != startRow;
}

bool Pawn::isTransformable(){
    return true;
}

bool Pawn::isCopyable(){
    return false;
}
