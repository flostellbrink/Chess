#ifndef ROOK_H
#define ROOK_H

#include "src/objects/Piece.h"

class Rook : public Piece
{
public:
  Rook(Board* board, int objectId, Field* field);
  std::vector<MoveBase*> GetMoves() override;
};


#endif // ROOK_H
