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
    if (field->Up) {
      if (!field->Up->CurrentPiece) {
        result.push_back(new Move(this, field->Up));
      }
      if (!UsedDoubleMove()
        && field->Up->Up
        && !field->Up->Up->CurrentPiece
        && !field->Up->CurrentPiece) {
        const auto move = new Move(this, field->Up->Up);
        move->en_passant_field = field->Up;
        move->is_en_passant = true;
        result.push_back(move);
      }
      if (field->Up->Right
        && field->Up->Right->CurrentPiece
        && field->Up->Right->CurrentPiece->IsWhite() != IsWhite()) {
        result.push_back(new Hit(this, field->Up->Right->CurrentPiece));
      }
      if (field->Up->Left && field->Up->Left->CurrentPiece
        && field->Up->Left->CurrentPiece->IsWhite() != IsWhite()) {
        result.push_back(new Hit(this, field->Up->Left->CurrentPiece));
      }
      if (lastMove
        && lastMove->is_en_passant
        && lastMove->en_passant_field
        && lastMove->main_piece
        && lastMove->main_piece->IsWhite() != IsWhite()
        && field->Up->Right
        && !field->Up->Right->CurrentPiece
        && lastMove->en_passant_field == field->Up->Right) {
        result.push_back(new Hit(this, lastMove->main_piece, lastMove->en_passant_field));
      }
      if (lastMove
        && lastMove->is_en_passant
        && lastMove->en_passant_field
        && lastMove->main_piece
        && lastMove->main_piece->IsWhite() != IsWhite()
        && field->Up->Left
        && !field->Up->Left->CurrentPiece
        && lastMove->en_passant_field == field->Up->Left) {
        result.push_back(new Hit(this, lastMove->main_piece, lastMove->en_passant_field));
      }
    }
  }
  else {
    if (field->Down) {
      if (!field->Down->CurrentPiece) {
        result.push_back(new Move(this, field->Down));
      }
      if (!UsedDoubleMove()
        && field->Down->Down
        && !field->Down->Down->CurrentPiece
        && !field->Down->CurrentPiece) {
        const auto move = new Move(this, field->Down->Down);
        move->en_passant_field = field->Down;
        move->is_en_passant = true;
        result.push_back(move);
      }
      if (field->Down->Right
        && field->Down->Right->CurrentPiece
        && field->Down->Right->CurrentPiece->IsWhite() != IsWhite()) {
        result.push_back(new Hit(this, field->Down->Right->CurrentPiece));
      }
      if (field->Down->Left
        && field->Down->Left->CurrentPiece
        && field->Down->Left->CurrentPiece->IsWhite() != IsWhite()) {
        result.push_back(new Hit(this, field->Down->Left->CurrentPiece));
      }
      if (lastMove
        && lastMove->is_en_passant
        && lastMove->en_passant_field
        && lastMove->main_piece
        && lastMove->main_piece->IsWhite() != IsWhite()
        && field->Down->Right
        && !field->Down->Right->CurrentPiece
        && lastMove->en_passant_field == field->Down->Right) {
        result.push_back(new Hit(this, lastMove->main_piece, lastMove->en_passant_field));
      }
      if (lastMove
        && lastMove->is_en_passant
        && lastMove->en_passant_field
        && lastMove->main_piece
        && lastMove->main_piece->IsWhite() != IsWhite()
        && field->Down->Left
        && !field->Down->Left->CurrentPiece
        && lastMove->en_passant_field == field->Down->Left) {
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
