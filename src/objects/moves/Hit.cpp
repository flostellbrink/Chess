#include "Hit.h"
#include "src/objects/Piece.h"
#include "src/objects/Board.h"


Hit::Hit(Piece *agressor, Piece *victim) {
  main_piece = agressor;
  agressor_ = agressor;
  victim_ = victim;
  to_field_ = victim->field;
  click_field = victim->field;
  field_overlay = 2;
}

Hit::Hit(Piece *agressor, Piece *victim, Field *toField) {
  main_piece = agressor;
  agressor_ = agressor;
  victim_ = victim;
  to_field_ = toField;
  click_field = toField;
  field_overlay = 2;
}

void Hit::Apply(Board* board, const bool sim) {
  from_field_ = agressor_->GetField();
  victim_from_ = victim_->GetField();
  victim_moved_ = victim_->is_moved;

  victim_->SetField(board->GetSideField(victim_->IsWhite()), sim);
  victim_->is_moved = true;
  is_moved_ = agressor_->is_moved;
  agressor_->SetField(to_field_, sim);
  agressor_->is_moved = true;
}

void Hit::Undo(Board* board, const bool sim) {
  (void)board;

  agressor_->SetField(from_field_, sim);
  agressor_->is_moved = is_moved_;
  victim_->SetField(victim_from_, sim);
  victim_->is_moved = victim_moved_;
}
