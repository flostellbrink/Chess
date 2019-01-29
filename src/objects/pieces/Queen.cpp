#include "Queen.h"
#include "src/objects/ObjectManager.h"
#include "src/objects/moves/Move.h"
#include "src/objects/Field.h"


Queen::Queen(Board* board, int objectID, Field *field) : Piece(board, objectID, field) {

}

std::vector<MoveBase*> Queen::GetMoves() {
  std::vector<MoveBase*> result;
  Field *currentField = field;
  while ((currentField = currentField->up)) {
    AddHitOrMove(currentField, result);
    if (currentField->current_piece)
      break;
  }
  currentField = field;
  while ((currentField = currentField->down)) {
    AddHitOrMove(currentField, result);
    if (currentField->current_piece)
      break;
  }
  currentField = field;
  while ((currentField = currentField->left)) {
    AddHitOrMove(currentField, result);
    if (currentField->current_piece)
      break;
  }
  currentField = field;
  while ((currentField = currentField->right)) {
    AddHitOrMove(currentField, result);
    if (currentField->current_piece)
      break;
  }
  currentField = field;
  while (currentField->up && (currentField = currentField->up->right)) {
    AddHitOrMove(currentField, result);
    if (currentField->current_piece)
      break;
  }
  currentField = field;
  while (currentField->down && (currentField = currentField->down->right)) {
    AddHitOrMove(currentField, result);
    if (currentField->current_piece)
      break;
  }
  currentField = field;
  while (currentField->up && (currentField = currentField->up->left)) {
    AddHitOrMove(currentField, result);
    if (currentField->current_piece)
      break;
  }
  currentField = field;
  while (currentField->down && (currentField = currentField->down->left)) {
    AddHitOrMove(currentField, result);
    if (currentField->current_piece)
      break;
  }
  return result;
}
