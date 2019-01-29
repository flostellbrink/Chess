#include "Rook.h"
#include "src/objects/ObjectManager.h"
#include "src/objects/moves/Move.h"
#include "src/objects/Field.h"

Rook::Rook(Board* board, const int objectId, Field* field) : Piece(board, objectId, field)
{
}

std::vector<MoveBase*> Rook::GetMoves()
{
  std::vector<MoveBase*> result;

  auto currentField = field;
  while ((currentField = currentField->up))
  {
    AddHitOrMove(currentField, result);
    if (currentField->current_piece)
    {
      break;
    }
  }

  currentField = field;
  while ((currentField = currentField->down))
  {
    AddHitOrMove(currentField, result);
    if (currentField->current_piece)
    {
      break;
    }
  }

  currentField = field;
  while ((currentField = currentField->left))
  {
    AddHitOrMove(currentField, result);
    if (currentField->current_piece)
    {
      break;
    }
  }

  currentField = field;
  while ((currentField = currentField->right))
  {
    AddHitOrMove(currentField, result);
    if (currentField->current_piece)
    {
      break;
    }
  }

  return result;
}
