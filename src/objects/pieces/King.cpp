#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.inl>

#include "King.h"
#include "src/objects/ObjectManager.h"
#include "src/objects/moves/Rochade.h"
#include "src/objects/Field.h"
#include "src/objects/Board.h"


King::King(Board* board, int objectID, Field *field) : Piece(board, objectID, field) {

}

void King::Init() {
  Piece::Init();
  y_rotation_ = glm::pi<float>() / 2.f;
}

std::vector<MoveBase*> King::GetMoves() {
  std::vector<MoveBase*> result;
  AddHitOrMove(field->Up, result);
  AddHitOrMove(field->Down, result);
  AddHitOrMove(field->Left, result);
  AddHitOrMove(field->Right, result);
  if (field->Up) {
    AddHitOrMove(field->Up->Left, result);
    AddHitOrMove(field->Up->Right, result);
  }
  if (field->Down) {
    AddHitOrMove(field->Down->Left, result);
    AddHitOrMove(field->Down->Right, result);
  }
  if (board_->IsRochadePossible(IsWhite(), true))
    result.push_back(new Rochade(this, board_->GetRook(IsWhite(), true), field->Left->Left, field->Left));
  if (board_->IsRochadePossible(IsWhite(), false))
    result.push_back(new Rochade(this, board_->GetRook(IsWhite(), false), field->Right->Right, field->Right));
  return result;
}

bool King::IsCopyable() {
  return false;
}
