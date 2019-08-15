#ifndef PAWN_H
#define PAWN_H

#include "src/objects/Piece.h"
#include <vector>

class Pawn : public Piece
{
public:
  Pawn(Board& board, int objectId, Field& field);
  std::vector<std::shared_ptr<MoveBase>> GetMoves() override;
  bool UsedDoubleMove();
  bool IsTransformable() const override;
  bool IsCopyable() const override;
};


#endif // PAWN_H
