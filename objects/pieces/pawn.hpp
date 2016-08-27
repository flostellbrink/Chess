// Abstract Class for all pieces
#ifndef PAWN_H
#define PAWN_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "objects/field.hpp"
#include "objects/piece.hpp"

class Pawn: public Piece{
    public:
    Pawn(Board* board, int objectID, Field* field);
    virtual vector<MoveBase*> GetMoves() override;
    bool UsedDoubleMove();
    virtual bool isTransformable() override;
    virtual bool isCopyable() override;

    protected:

    private:
};


#endif // PAWN_H
