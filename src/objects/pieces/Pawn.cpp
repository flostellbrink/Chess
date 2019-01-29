#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Pawn.h"
#include "src/objects/moves/Move.h"
#include "src/objects/moves/Hit.h"
#include "src/objects/Field.h"
#include "src/objects/Board.h"


Pawn::Pawn(Board* board, const int objectId, Field* field) : Piece(board, objectId, field) {

}

std::vector<MoveBase*> Pawn::GetMoves() {
  MoveBase* lastMove = board_->GetLastMove();
  std::vector<MoveBase*> result;
  if (IsWhite()) {
    if (field->up) {
      if (!field->up->current_piece) {
        result.push_back(new Move(this, field->up));
      }
      if (!UsedDoubleMove()
        && field->up->up
        && !field->up->up->current_piece
        && !field->up->current_piece) {
        const auto move = new Move(this, field->up->up);
        move->en_passant_field = field->up;
        move->is_en_passant = true;
        result.push_back(move);
      }
      if (field->up->right
        && field->up->right->current_piece
        && field->up->right->current_piece->IsWhite() != IsWhite()) {
        result.push_back(new Hit(this, field->up->right->current_piece));
      }
      if (field->up->left && field->up->left->current_piece
        && field->up->left->current_piece->IsWhite() != IsWhite()) {
        result.push_back(new Hit(this, field->up->left->current_piece));
      }
      if (lastMove
        && lastMove->is_en_passant
        && lastMove->en_passant_field
        && lastMove->main_piece
        && lastMove->main_piece->IsWhite() != IsWhite()
        && field->up->right
        && !field->up->right->current_piece
        && lastMove->en_passant_field == field->up->right) {
        result.push_back(new Hit(this, lastMove->main_piece, lastMove->en_passant_field));
      }
      if (lastMove
        && lastMove->is_en_passant
        && lastMove->en_passant_field
        && lastMove->main_piece
        && lastMove->main_piece->IsWhite() != IsWhite()
        && field->up->left
        && !field->up->left->current_piece
        && lastMove->en_passant_field == field->up->left) {
        result.push_back(new Hit(this, lastMove->main_piece, lastMove->en_passant_field));
      }
    }
  }
  else {
    if (field->down) {
      if (!field->down->current_piece) {
        result.push_back(new Move(this, field->down));
      }
      if (!UsedDoubleMove()
        && field->down->down
        && !field->down->down->current_piece
        && !field->down->current_piece) {
        const auto move = new Move(this, field->down->down);
        move->en_passant_field = field->down;
        move->is_en_passant = true;
        result.push_back(move);
      }
      if (field->down->right
        && field->down->right->current_piece
        && field->down->right->current_piece->IsWhite() != IsWhite()) {
        result.push_back(new Hit(this, field->down->right->current_piece));
      }
      if (field->down->left
        && field->down->left->current_piece
        && field->down->left->current_piece->IsWhite() != IsWhite()) {
        result.push_back(new Hit(this, field->down->left->current_piece));
      }
      if (lastMove
        && lastMove->is_en_passant
        && lastMove->en_passant_field
        && lastMove->main_piece
        && lastMove->main_piece->IsWhite() != IsWhite()
        && field->down->right
        && !field->down->right->current_piece
        && lastMove->en_passant_field == field->down->right) {
        result.push_back(new Hit(this, lastMove->main_piece, lastMove->en_passant_field));
      }
      if (lastMove
        && lastMove->is_en_passant
        && lastMove->en_passant_field
        && lastMove->main_piece
        && lastMove->main_piece->IsWhite() != IsWhite()
        && field->down->left
        && !field->down->left->current_piece
        && lastMove->en_passant_field == field->down->left) {
        result.push_back(new Hit(this, lastMove->main_piece, lastMove->en_passant_field));
      }
    }
  }
  return result;
}

bool Pawn::UsedDoubleMove() {
  const auto startRow = IsWhite() ? 1 : 6;
  return field->Row() != startRow;
}

bool Pawn::IsTransformable() {
  return true;
}

bool Pawn::IsCopyable() {
  return false;
}
