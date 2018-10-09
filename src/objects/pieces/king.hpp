#ifndef KING_H
#define KING_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "objects/field.hpp"
#include "objects/piece.hpp"

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
