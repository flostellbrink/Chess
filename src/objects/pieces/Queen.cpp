#include "Queen.h"
#include "src/objects/ObjectManager.h"
#include "src/objects/moves/Move.h"
#include "src/objects/Field.h"


Queen::Queen(Board* board, int objectID, Field *field) : Piece(board, objectID, field) {

}

std::vector<MoveBase*> Queen::GetMoves() {
  std::vector<MoveBase*> result;
  Field *currentField = field;
  while ((currentField = currentField->Up)) {
    AddHitOrMove(currentField, result);
    if (currentField->CurrentPiece)
      break;
  }
  currentField = field;
  while ((currentField = currentField->Down)) {
    AddHitOrMove(currentField, result);
    if (currentField->CurrentPiece)
      break;
  }
  currentField = field;
  while ((currentField = currentField->Left)) {
    AddHitOrMove(currentField, result);
    if (currentField->CurrentPiece)
      break;
  }
  currentField = field;
  while ((currentField = currentField->Right)) {
    AddHitOrMove(currentField, result);
    if (currentField->CurrentPiece)
      break;
  }
  currentField = field;
  while (currentField->Up && (currentField = currentField->Up->Right)) {
    AddHitOrMove(currentField, result);
    if (currentField->CurrentPiece)
      break;
  }
  currentField = field;
  while (currentField->Down && (currentField = currentField->Down->Right)) {
    AddHitOrMove(currentField, result);
    if (currentField->CurrentPiece)
      break;
  }
  currentField = field;
  while (currentField->Up && (currentField = currentField->Up->Left)) {
    AddHitOrMove(currentField, result);
    if (currentField->CurrentPiece)
      break;
  }
  currentField = field;
  while (currentField->Down && (currentField = currentField->Down->Left)) {
    AddHitOrMove(currentField, result);
    if (currentField->CurrentPiece)
      break;
  }
  return result;
}
