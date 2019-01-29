#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.inl>

#include "King.h"
#include "src/objects/ObjectManager.h"
#include "src/objects/moves/Castling.h"
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
  AddHitOrMove(field->up, result);
  AddHitOrMove(field->down, result);
  AddHitOrMove(field->left, result);
  AddHitOrMove(field->right, result);
  if (field->up) {
    AddHitOrMove(field->up->left, result);
    AddHitOrMove(field->up->right, result);
  }
  if (field->down) {
    AddHitOrMove(field->down->left, result);
    AddHitOrMove(field->down->right, result);
  }
  if (board_->IsCastlingPossible(IsWhite(), true))
    result.push_back(new Castling(this, board_->GetRook(IsWhite(), true), field->left->left, field->left));
  if (board_->IsCastlingPossible(IsWhite(), false))
    result.push_back(new Castling(this, board_->GetRook(IsWhite(), false), field->right->right, field->right));
  return result;
}

bool King::IsCopyable() {
  return false;
}
