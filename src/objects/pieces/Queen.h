#ifndef QUEEN_H
#define QUEEN_H

#include "src/objects/Piece.h"

class Queen : public Piece
{
public:
  Queen(Board* board, int objectId, Field* field);
  std::vector<std::shared_ptr<MoveBase>> GetMoves() override;
};


#endif // QUEEN_H
