#ifndef HIT_H
#define HIT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/objects/board.hpp"
#include "moveBase.hpp"

class Hit : public MoveBase {
    public:
    Hit(Piece* agressor, Piece* victim);
    Hit(Piece *agressor, Piece *victim, Field *toField);
    virtual void Apply(Board* board, bool sim = false) override;
    virtual void Undo(Board* board, bool sim = false) override;

    protected:
    bool _victimMoved;

    private:
    Piece *_agressor, *_victim;
    Field *_fromField, *_toField, *_victimFrom;
};


#endif // HIT_H
