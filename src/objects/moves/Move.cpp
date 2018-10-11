#include "Move.h"
#include "src/objects/Piece.h"


Move::Move(Piece *piece, Field *toField) {
  main_piece = piece;
  piece_ = piece;
  to_field_ = toField;
  click_field = toField;
}

void Move::Apply(Board* board, const bool sim) {
  (void)board;

  from_field_ = piece_->GetField();
  is_moved_ = piece_->is_moved;
  piece_->SetField(to_field_, sim);
  piece_->is_moved = true;
}

void Move::Undo(Board* board, const bool sim) {
  (void)board;

  piece_->SetField(from_field_, sim);
  piece_->is_moved = is_moved_;
}
