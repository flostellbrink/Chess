#ifndef BISHOP_H
#define BISHOP_H

#include "src/objects/Piece.h"

class Bishop : public Piece
{
public:
  Bishop(Board* board, int objectId, Field* field);
  std::vector<MoveBase*> GetMoves() override;
};


#endif // BISHOP_H
