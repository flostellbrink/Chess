#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/objects/board.hpp"
#include "moveBase.hpp"

class Transform : public MoveBase {
    public:
    Transform(Piece *piece, Piece *transformInto);
    void Apply(Board* board, bool sim) override;
    void Undo(Board* board, bool sim) override;
    bool ChangeTurn() override;

    protected:

    private:
    Piece *_piece, *_transformInto;
    Field *_fromField, *_toField;
};


#endif // TRANSFORM_H
