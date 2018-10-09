#ifndef ROOK_H
#define ROOK_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "objects/field.hpp"
#include "objects/piece.hpp"

class Rook: public Piece{
    public:
    Rook (Board* board, int objectID, Field* field);
    virtual vector<MoveBase*> GetMoves() override;

    protected:

    private:
};


#endif // ROOK_H
