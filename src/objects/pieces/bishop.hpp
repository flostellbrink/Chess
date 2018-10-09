#ifndef BISHOP_H
#define BISHOP_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "objects/field.hpp"
#include "objects/piece.hpp"

class Bishop: public Piece{
    public:
    Bishop (Board* board, int objectID, Field* field);
    virtual vector<MoveBase*> GetMoves() override;

    protected:

    private:
};


#endif // BISHOP_H
