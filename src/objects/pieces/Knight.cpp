#include <glm/glm.hpp>
#include <glm/gtc/constants.inl>

#include "Knight.h"
#include "src/objects/ObjectManager.h"
#include "src/objects/moves/Move.h"
#include "src/objects/Field.h"


Knight::Knight(Board* board, const int objectId, Field *field) : Piece(board, objectId, field) {

}

void Knight::Init() {
  Piece::Init();
  if (!IsWhite())
    y_rotation_ = glm::pi<float>();
}

std::vector<MoveBase*> Knight::GetMoves() {
  std::vector<MoveBase*> result;

  if (field->Up && field->Up->Up) {
    const auto current = field->Up->Up;
    AddHitOrMove(current->Left, result);
    AddHitOrMove(current->Right, result);
  }

  if (field->Down && field->Down->Down) {
    auto const current = field->Down->Down;
    AddHitOrMove(current->Left, result);
    AddHitOrMove(current->Right, result);
  }

  if (field->Left && field->Left->Left) {
    auto const current = field->Left->Left;
    AddHitOrMove(current->Up, result);
    AddHitOrMove(current->Down, result);
  }

  if (field->Right && field->Right->Right) {
    auto const current = field->Right->Right;
    AddHitOrMove(current->Up, result);
    AddHitOrMove(current->Down, result);
  }

  return result;
}
