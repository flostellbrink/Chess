#ifndef BISHOP_H
#define BISHOP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/objects/field.hpp"
#include "src/objects/piece.hpp"

class Bishop: public Piece{
    public:
    Bishop (Board* board, int objectID, Field* field);
    virtual vector<MoveBase*> GetMoves() override;

    protected:

    private:
};


#endif // BISHOP_H
