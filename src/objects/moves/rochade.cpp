#include <GL/glew.h>

#include "rochade.hpp"

using glm::vec3;
using glm::mat4;

Rochade::Rochade(Piece* king, Piece* rook, Field* toKing, Field* toRook){
    _king = king;
    _rook = rook;
    _toKing = toKing;
    _toRook = toRook;
    MainPiece = king;
    ClickField = toKing;
}

void Rochade::Apply(Board* board, bool sim){
    // Ignore parameter unused
    (void)board;

    _fromKing = _king->GetField();
    _fromRook = _rook->GetField();
    _king->SetField(_toKing, sim);
    _rook->SetField(_toRook, sim);
    _king->IsMoved = true;
    _rook->IsMoved = true;
}

void Rochade::Undo(Board* board, bool sim){
    // Ignore parameter unused
    (void)board;

    _king->SetField(_fromKing, sim);
    _rook->SetField(_fromRook, sim);
    _king->IsMoved = false;
    _rook->IsMoved = false;
}
