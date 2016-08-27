#ifndef TRANSFORM_H
#define TRANSFORM_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include "objects/board.hpp"
#include "moveBase.hpp"

class Transform : public MoveBase {
    public:
    Transform(Piece *piece, Piece *transformInto);
    virtual void Apply(Board* board, bool sim = false) override;
    virtual void Undo(Board* board, bool sim = false) override;
    virtual bool ChangeTurn() override;

    protected:

    private:
    Piece *_piece, *_transformInto;
    Field *_fromField, *_toField;
};


#endif // TRANSFORM_H
