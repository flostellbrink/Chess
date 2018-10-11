#ifndef PAWN_H
#define PAWN_H

#include "src/objects/Piece.h"
#include <vector>

class Pawn : public Piece {
public:
  Pawn(Board* board, int objectId, Field* field);
  std::vector<MoveBase*> GetMoves() override;
  bool UsedDoubleMove();
  bool IsTransformable() override;
  bool IsCopyable() override;
};


#endif // PAWN_H
