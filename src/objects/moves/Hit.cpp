#include "Hit.h"
#include "src/objects/Piece.h"
#include "src/objects/Board.h"


Hit::Hit(Piece *aggressor, Piece *victim) {
  main_piece = aggressor;
  aggressor_ = aggressor;
  victim_ = victim;
  to_field_ = victim->field;
  click_field = victim->field;
  field_overlay = 2;
}

Hit::Hit(Piece *aggressor, Piece *victim, Field *toField) {
  main_piece = aggressor;
  aggressor_ = aggressor;
  victim_ = victim;
  to_field_ = toField;
  click_field = toField;
  field_overlay = 2;
}

void Hit::Apply(Board* board, const bool sim) {
  from_field_ = aggressor_->GetField();
  victim_from_ = victim_->GetField();
  victim_moved_ = victim_->is_moved;

  victim_->SetField(board->GetSideField(victim_->IsWhite()), sim);
  victim_->is_moved = true;
  is_moved_ = aggressor_->is_moved;
  aggressor_->SetField(to_field_, sim);
  aggressor_->is_moved = true;
}

void Hit::Undo(Board* board, const bool sim) {
  (void)board;

  aggressor_->SetField(from_field_, sim);
  aggressor_->is_moved = is_moved_;
  victim_->SetField(victim_from_, sim);
  victim_->is_moved = victim_moved_;
}
