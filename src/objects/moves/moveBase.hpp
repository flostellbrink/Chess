#ifndef MOVEBASE_H
#define MOVEBASE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/objects/board.hpp"

class Field;
class Board;
class Piece;

///
/// This represents one move on the chess board.
/// It is created from a field, validated by the board and called by a field when it's clicked on
///
class MoveBase {
    public:
    MoveBase();
    virtual void Apply(Board* board, bool sim = false) = 0;
    virtual void Undo(Board* board, bool sim = false) = 0;
    virtual bool ChangeTurn();
    Field *ClickField, *EnPassantField = 0;
    Piece* MainPiece = 0;
    int FieldOverlay = 1;
    bool IsEnPassant = false;

    protected:
    bool _isMoved;

    private:
};


#endif // MOVEBASE_H
