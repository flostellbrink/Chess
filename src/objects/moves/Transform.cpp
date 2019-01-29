#include "Transform.h"
#include "src/objects/Piece.h"
#include "src/objects/Board.h"
#include "src/objects/Field.h"


Transform::Transform(Piece *piece, Piece *transformInto) {
  main_piece = piece;
  piece_ = piece;
  transform_into_ = transformInto;
  click_field= transformInto->field;
}

void Transform::Apply(Board* board, bool sim) {
  // Ignore isMoved, because pieces are only transformed when they already have moved
  from_field_ = piece_->GetField();
  to_field_ = board->GetSideField(piece_->IsWhite());
  piece_->SetField(to_field_, sim);
  board->AddPiece((transform_into_->GetIdWithoutColor() + 1 - piece_->IsWhite()), from_field_);
}

void Transform::Undo(Board* board, bool sim) {
  from_field_->current_piece->SetField(board->GetSideField(piece_->IsWhite()));
  piece_->SetField(from_field_, sim);
}

bool Transform::ChangeTurn() {
  return false;
}
