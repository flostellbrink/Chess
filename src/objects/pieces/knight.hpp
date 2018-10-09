#ifndef KNIGHT_H
#define KNIGHT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/objects/field.hpp"
#include "src/objects/piece.hpp"

class Knight: public Piece{
    public:
    Knight (Board* board, int objectID, Field* field);
    virtual void init() override;
    virtual vector<MoveBase*> GetMoves() override;

    protected:

    private:
};


#endif // KNIGHT_H
