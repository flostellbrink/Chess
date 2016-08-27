#ifndef KNIGHT_H
#define KNIGHT_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "objects/field.hpp"
#include "objects/piece.hpp"

class Knight: public Piece{
    public:
    Knight (Board* board, int objectID, Field* field);
    virtual void init() override;
    virtual vector<MoveBase*> GetMoves() override;

    protected:

    private:
};


#endif // KNIGHT_H
