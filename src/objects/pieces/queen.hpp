#ifndef QUEEN_H
#define QUEEN_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "objects/field.hpp"
#include "objects/piece.hpp"

class Queen: public Piece{
    public:
    Queen (Board* board, int objectID, Field* field);
    virtual vector<MoveBase*> GetMoves() override;

    protected:

    private:
};


#endif // QUEEN_H
