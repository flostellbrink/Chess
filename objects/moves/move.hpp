#ifndef MOVE_H
#define MOVE_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include "objects/board.hpp"
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
