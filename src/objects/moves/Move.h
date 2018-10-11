#ifndef MOVE_H
#define MOVE_H

#include "MoveBase.h"

class Move : public MoveBase {
public:
  Move(Piece* piece, Field* toField);
  void Apply(Board* board, bool sim) override;
  void Undo(Board* board, bool sim) override;

private:
  Piece *piece_;
  Field *from_field_{}, *to_field_;
};


#endif // MOVE_H
