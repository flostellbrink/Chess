#include "Castling.h"
#include "src/objects/Piece.h"


Castling::Castling(Piece* king, Piece* rook, Field* toKing, Field* toRook)
{
  king_ = king;
  rook_ = rook;
  to_king_ = toKing;
  to_rook_ = toRook;
  main_piece = king;
  click_field = toKing;
}

void Castling::Apply(Board* board, const bool simulate)
{
  (void)board;

  from_king_ = king_->GetField();
  from_rook_ = rook_->GetField();
  king_->SetField(to_king_, simulate);
  rook_->SetField(to_rook_, simulate);
  king_->is_moved = true;
  rook_->is_moved = true;
}

void Castling::Undo(Board* board, const bool simulate)
{
  (void)board;

  king_->SetField(from_king_, simulate);
  rook_->SetField(from_rook_, simulate);
  king_->is_moved = false;
  rook_->is_moved = false;
}
