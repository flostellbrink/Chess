#include "Rochade.h"
#include "src/objects/Piece.h"


Rochade::Rochade(Piece* king, Piece* rook, Field* toKing, Field* toRook) {
  king_ = king;
  rook_ = rook;
  to_king_ = toKing;
  to_rook_ = toRook;
  main_piece = king;
  click_field = toKing;
}

void Rochade::Apply(Board* board, bool sim) {
  (void)board;

  from_king_ = king_->GetField();
  from_rook_ = rook_->GetField();
  king_->SetField(to_king_, sim);
  rook_->SetField(to_rook_, sim);
  king_->is_moved = true;
  rook_->is_moved = true;
}

void Rochade::Undo(Board* board, bool sim) {
  (void)board;

  king_->SetField(from_king_, sim);
  rook_->SetField(from_rook_, sim);
  king_->is_moved = false;
  rook_->is_moved = false;
}
