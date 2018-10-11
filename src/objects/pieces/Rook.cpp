#include "Rook.h"
#include "src/objects/ObjectManager.h"
#include "src/objects/moves/Move.h"
#include "src/objects/Field.h"

Rook::Rook(Board* board, int objectID, Field *field) : Piece(board, objectID, field) {

}

std::vector<MoveBase*> Rook::GetMoves() {
  std::vector<MoveBase*> result;

  auto currentField = field;
  while ((currentField = currentField->Up)) {
    AddHitOrMove(currentField, result);
    if (currentField->CurrentPiece) {
      break;
    }
  }

  currentField = field;
  while ((currentField = currentField->Down)) {
    AddHitOrMove(currentField, result);
    if (currentField->CurrentPiece) {
      break;
    }
  }

  currentField = field;
  while ((currentField = currentField->Left)) {
    AddHitOrMove(currentField, result);
    if (currentField->CurrentPiece) {
      break;
    }
  }

  currentField = field;
  while ((currentField = currentField->Right)) {
    AddHitOrMove(currentField, result);
    if (currentField->CurrentPiece) {
      break;
    }
  }

  return result;
}
