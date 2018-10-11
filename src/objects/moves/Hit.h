#ifndef HIT_H
#define HIT_H

#include "MoveBase.h"

class Hit : public MoveBase {
public:
  Hit(Piece* agressor, Piece* victim);
  Hit(Piece *agressor, Piece *victim, Field *toField);
  void Apply(Board* board, bool sim) override;
  void Undo(Board* board, bool sim) override;

protected:
  bool victim_moved_{};

private:
  Piece *agressor_, *victim_;
  Field *from_field_{}, *to_field_, *victim_from_{};
};


#endif // HIT_H
