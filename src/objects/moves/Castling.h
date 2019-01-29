#ifndef CASTLING_H
#define CASTLING_H

#include "MoveBase.h"

class Castling : public MoveBase {
public:
  Castling(Piece* king, Piece* rook, Field* toKing, Field* toRook);
  void Apply(Board* board, bool) override;
  void Undo(Board* board, bool) override;

private:
  Piece *king_, *rook_;
  Field *from_king_{}, *to_king_, *from_rook_{}, *to_rook_;
};


#endif // CASTLING_H
