#include <GL/glew.h>

#include "transform.hpp"
#include <iostream>

using glm::vec3;
using glm::mat4;

Transform::Transform(Piece *piece, Piece *transformInto){
    MainPiece = piece;
    _piece = piece;
    _transformInto = transformInto;
    ClickField = transformInto->_field;
}

void Transform::Apply(Board* board, bool sim){
    // Ignore isMoved, because pieces are only transformed when they already have moved
    _fromField = _piece->GetField();
    _toField = board->GetSideField(_piece->isWhite());
    _piece->SetField(_toField, sim);
    board->AddPiece((uint) (_transformInto->GetIdWithoutColor() + 1 - _piece->isWhite()), _fromField);
}

void Transform::Undo(Board* board, bool sim){
    _fromField->CurrentPiece->SetField(board->GetSideField(_piece->isWhite()));
    _piece->SetField(_fromField, sim);
}

bool Transform::ChangeTurn(){
    return false;
}
