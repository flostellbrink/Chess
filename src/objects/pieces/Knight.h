#ifndef KNIGHT_H
#define KNIGHT_H

#include <vector>
#include "src/objects/Piece.h"

class Knight : public Piece {
public:
  Knight(Board* board, int objectId, Field* field);
  void Init() override;
  std::vector<MoveBase*> GetMoves() override;
};


#endif // KNIGHT_H
