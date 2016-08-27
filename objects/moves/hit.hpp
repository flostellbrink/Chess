#ifndef HIT_H
#define HIT_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include "objects/board.hpp"
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
