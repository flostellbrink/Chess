#ifndef ROOK_H
#define ROOK_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/objects/field.hpp"
#include "src/objects/piece.hpp"

class Rook: public Piece{
    public:
    Rook (Board* board, int objectID, Field* field);
    virtual vector<MoveBase*> GetMoves() override;

    protected:

    private:
};


#endif // ROOK_H
