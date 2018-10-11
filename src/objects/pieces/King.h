#ifndef KING_H
#define KING_H

#include <vector>
#include "src/objects/Piece.h"

class King : public Piece {
public:
  King(Board* board, int objectId, Field* field);
  void Init() override;
  std::vector<MoveBase*> GetMoves() override;
  bool IsCopyable() override;
};


#endif // KING_H
