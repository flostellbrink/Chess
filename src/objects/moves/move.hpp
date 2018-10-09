#ifndef MOVE_H
#define MOVE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/objects/board.hpp"
#include "moveBase.hpp"

class Move : public MoveBase {
    public:
    Move(Piece* piece, Field* toField);
    virtual void Apply(Board* board, bool sim = false) override;
    virtual void Undo(Board* board, bool sim = false) override;

    protected:

    private:
    Piece *_piece;
    Field *_fromField, *_toField;
};


#endif // MOVE_H
