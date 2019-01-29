#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.inl>

#include "Knight.h"
#include "src/objects/ObjectManager.h"
#include "src/objects/moves/Move.h"
#include "src/objects/Field.h"


Knight::Knight(Board* board, const int objectId, Field* field) : Piece(board, objectId, field)
{
}

void Knight::Init()
{
  Piece::Init();
  if (!IsWhite())
    y_rotation_ = glm::pi<float>();
}

std::vector<MoveBase*> Knight::GetMoves()
{
  std::vector<MoveBase*> result;

  if (field->up && field->up->up)
  {
    const auto current = field->up->up;
    AddHitOrMove(current->left, result);
    AddHitOrMove(current->right, result);
  }

  if (field->down && field->down->down)
  {
    auto const current = field->down->down;
    AddHitOrMove(current->left, result);
    AddHitOrMove(current->right, result);
  }

  if (field->left && field->left->left)
  {
    auto const current = field->left->left;
    AddHitOrMove(current->up, result);
    AddHitOrMove(current->down, result);
  }

  if (field->right && field->right->right)
  {
    auto const current = field->right->right;
    AddHitOrMove(current->up, result);
    AddHitOrMove(current->down, result);
  }

  return result;
}
