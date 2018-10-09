// Abstract Class for all pieces
#ifndef PAWN_H
#define PAWN_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/objects/field.hpp"
#include "src/objects/piece.hpp"

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
