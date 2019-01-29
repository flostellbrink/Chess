#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "MoveBase.h"

class Transform : public MoveBase
{
public:
  Transform(Piece* piece, Piece* transformInto);
  void Apply(Board* board, bool simulate) override;
  void Undo(Board* board, bool simulate) override;
  bool ChangeTurn() override;

private:
  Piece *piece_, *transform_into_;
  Field *from_field_{}, *to_field_{};
};


#endif // TRANSFORM_H
