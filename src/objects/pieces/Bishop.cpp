#include "Bishop.h"
#include "src/objects/ObjectManager.h"
#include "src/objects/moves/Move.h"
#include "src/objects/Field.h"


Bishop::Bishop(Board& board, const int objectId, Field& field) : Piece(board, objectId, field)
{
}

std::vector<std::shared_ptr<MoveBase>> Bishop::GetMoves()
{
  std::vector<std::shared_ptr<MoveBase>> result;

  {
    auto currentField = field;
    while (currentField->up && currentField->up->right)
    {
      currentField = currentField->up->right;
      AddHitOrMove(currentField, result);
      if (currentField->current_piece)
      {
        break;
      }
    }
  }

  {
    auto currentField = field;
    while (currentField->down && currentField->down->right)
    {
      currentField = currentField->down->right;
      AddHitOrMove(currentField, result);
      if (currentField->current_piece)
      {
        break;
      }
    }
  }

  {
    auto currentField = field;
    while (currentField->up && currentField->up->left)
    {
      currentField = currentField->up->left;
      AddHitOrMove(currentField, result);
      if (currentField->current_piece)
      {
        break;
      }
    }
  }

  {
    auto currentField = field;
    while (currentField->down && currentField->down->left)
    {
      currentField = currentField->down->left;
      AddHitOrMove(currentField, result);
      if (currentField->current_piece)
      {
        break;
      }
    }
  }

  return result;
}
