#ifndef KING_H
#define KING_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/objects/field.hpp"
#include "src/objects/piece.hpp"

class King: public Piece{
    public:
    King (Board* board, int objectID, Field* field);
    virtual void init() override;
    virtual vector<MoveBase*> GetMoves() override;
    virtual bool isCopyable() override;

    protected:

    private:
};


#endif // KING_H
