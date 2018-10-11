#include "Bishop.h"
#include "src/objects/ObjectManager.h"
#include "src/objects/moves/Move.h"
#include "src/objects/Field.h"


Bishop::Bishop(Board* board, const int objectID, Field *field) : Piece(board, objectID, field) {

}

std::vector<MoveBase*> Bishop::GetMoves() {
  std::vector<MoveBase*> result;

  {
    auto currentField = field;
    while (currentField->Up && currentField->Up->Right) {
      currentField = currentField->Up->Right;
      AddHitOrMove(currentField, result);
      if (currentField->CurrentPiece) {
        break;
      }
    }
  }

  {
    auto currentField = field;
    while (currentField->Down && currentField->Down->Right) {
      currentField = currentField->Down->Right;
      AddHitOrMove(currentField, result);
      if (currentField->CurrentPiece) {
        break;
      }
    }
  }

  {
    auto currentField = field;
    while (currentField->Up && currentField->Up->Left) {
      currentField = currentField->Up->Left;
      AddHitOrMove(currentField, result);
      if (currentField->CurrentPiece) {
        break;
      }
    }
  }

  {
    auto currentField = field;
    while (currentField->Down && currentField->Down->Left) {
      currentField = currentField->Down->Left;
      AddHitOrMove(currentField, result);
      if (currentField->CurrentPiece) {
        break;
      }
    }
  }

  return result;
}
