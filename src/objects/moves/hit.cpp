#include <GL/glew.h>

#include "hit.hpp"

using glm::vec3;
using glm::mat4;

Hit::Hit(Piece *agressor, Piece *victim){
    MainPiece = agressor;
    _agressor = agressor;
    _victim = victim;
    _toField = victim->_field;
    ClickField = victim->_field;
    FieldOverlay = 2;
}

Hit::Hit(Piece *agressor, Piece *victim, Field *toField){
    MainPiece = agressor;
    _agressor = agressor;
    _victim = victim;
    _toField = toField;
    ClickField = toField;
    FieldOverlay = 2;
}

void Hit::Apply(Board* board, bool sim){
    _fromField = _agressor->GetField();
    _victimFrom = _victim->GetField();
    _victimMoved = _victim->IsMoved;

    _victim->SetField(board->GetSideField(_victim->isWhite()), sim);
    _victim->IsMoved = true;
    _isMoved = _agressor->IsMoved;
    _agressor->SetField(_toField, sim);
    _agressor->IsMoved = true;
}

void Hit::Undo(Board* board, bool sim){
    // Ignore parameter unused
    (void)board;

    _agressor->SetField(_fromField, sim);
    _agressor->IsMoved = _isMoved;
    _victim->SetField(_victimFrom, sim);
    _victim->IsMoved = _victimMoved;
}
