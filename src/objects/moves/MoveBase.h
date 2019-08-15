#ifndef MOVE_BASE_H
#define MOVE_BASE_H

class Board;
class Field;
class Piece;

/**
 * This represents one move on the chess board.
 * It is created from a field, validated by the board and called by a field when it's clicked on
 */
class MoveBase
{
public:
  virtual ~MoveBase() = default;
  MoveBase();
  virtual void Apply(Board* board, bool sim) = 0;
  virtual void Undo(Board* board, bool sim) = 0;
  virtual bool ChangeTurn();
  Field* click_field = nullptr, *en_passant_field = nullptr;
  Piece* main_piece = nullptr;
  int field_overlay = 1;
  bool is_en_passant = false;

protected:
  bool is_moved_{};
};


#endif // MOVE_BASE_H
