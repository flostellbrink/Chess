#include <GL/glew.h>

#include "move.hpp"

using glm::vec3;
using glm::mat4;

Move::Move(Piece *piece, Field *toField){
    MainPiece = piece;
    _piece = piece;
    _toField = toField;
    ClickField = toField;
}

void Move::Apply(Board* board, bool sim){
    _fromField = _piece->GetField();
    _isMoved = _piece->IsMoved;
    _piece->SetField(_toField, sim);
    _piece->IsMoved = true;
}

void Move::Undo(Board* board, bool sim){
    _piece->SetField(_fromField, sim);
    _piece->IsMoved = _isMoved;
}
