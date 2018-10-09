#include <GL/glew.h>

#include "pawn.hpp"
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

Pawn::Pawn(Board* board, int objectID, Field* field) : Piece(board, objectID, field) {

}

vector<MoveBase*> Pawn::GetMoves(){
    MoveBase* lastMove = _board->GetLastMove();
    vector<MoveBase*> result;
    if(isWhite()){
        if(_field->Up){
            if(!_field->Up->CurrentPiece)
                result.push_back(new Move(this, _field->Up));
            if(!UsedDoubleMove() && _field->Up->Up && !_field->Up->Up->CurrentPiece && !_field->Up->CurrentPiece){
                Move* move = new Move(this, _field->Up->Up);
                move->EnPassantField = _field->Up;
                move->IsEnPassant = true;
                result.push_back(move);
            }
            if(_field->Up->Right && _field->Up->Right->CurrentPiece && _field->Up->Right->CurrentPiece->isWhite() != isWhite())
                result.push_back(new Hit(this, _field->Up->Right->CurrentPiece));
            if(_field->Up->Left && _field->Up->Left->CurrentPiece && _field->Up->Left->CurrentPiece->isWhite() != isWhite())
                result.push_back(new Hit(this, _field->Up->Left->CurrentPiece));
            if(lastMove && lastMove->IsEnPassant && lastMove->EnPassantField && lastMove->MainPiece && lastMove->MainPiece->isWhite() != isWhite() && _field->Up->Right && !_field->Up->Right->CurrentPiece && lastMove->EnPassantField == _field->Up->Right)
                result.push_back(new Hit(this, lastMove->MainPiece, lastMove->EnPassantField));
            if(lastMove && lastMove->IsEnPassant && lastMove->EnPassantField && lastMove->MainPiece && lastMove->MainPiece->isWhite() != isWhite() && _field->Up->Left && !_field->Up->Left->CurrentPiece && lastMove->EnPassantField == _field->Up->Left)
                result.push_back(new Hit(this, lastMove->MainPiece, lastMove->EnPassantField));
        }
    } else {
        if(_field->Down){
            if(!_field->Down->CurrentPiece)
                result.push_back(new Move(this, _field->Down));
            if(!UsedDoubleMove() &&_field->Down->Down && !_field->Down->Down->CurrentPiece && !_field->Down->CurrentPiece){
                Move* move = new Move(this, _field->Down->Down);
                move->EnPassantField = _field->Down;
                move->IsEnPassant = true;
                result.push_back(move);
            }
            if(_field->Down->Right && _field->Down->Right->CurrentPiece && _field->Down->Right->CurrentPiece->isWhite() != isWhite())
                result.push_back(new Hit(this, _field->Down->Right->CurrentPiece));
            if(_field->Down->Left && _field->Down->Left->CurrentPiece && _field->Down->Left->CurrentPiece->isWhite() != isWhite())
                result.push_back(new Hit(this, _field->Down->Left->CurrentPiece));
            if(lastMove && lastMove->IsEnPassant && lastMove->EnPassantField && lastMove->MainPiece && lastMove->MainPiece->isWhite() != isWhite() && _field->Down->Right && !_field->Down->Right->CurrentPiece && lastMove->EnPassantField == _field->Down->Right)
                result.push_back(new Hit(this, lastMove->MainPiece, lastMove->EnPassantField));
            if(lastMove && lastMove->IsEnPassant && lastMove->EnPassantField && lastMove->MainPiece && lastMove->MainPiece->isWhite() != isWhite() && _field->Down->Left && !_field->Down->Left->CurrentPiece && lastMove->EnPassantField == _field->Down->Left)
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
