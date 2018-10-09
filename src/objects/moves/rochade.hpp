#ifndef ROCHADE_H
#define ROCHADE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/objects/board.hpp"
#include "moveBase.hpp"

class Rochade : public MoveBase {
    public:
    Rochade(Piece* king, Piece* rook, Field* toKing, Field* toRook);
    virtual void Apply(Board* board, bool sim = false) override;
    virtual void Undo(Board* board, bool sim = false) override;

    protected:

    private:
    Piece *_king, *_rook;
    Field *_fromKing, *_toKing, *_fromRook, *_toRook;
};


#endif // ROCHADE_H
