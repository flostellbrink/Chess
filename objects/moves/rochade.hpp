#ifndef ROCHADE_H
#define ROCHADE_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include "objects/board.hpp"
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
