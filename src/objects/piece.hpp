// Abstract Class for all pieces
#ifndef PIECE_H
#define PIECE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "drawable.hpp"
#include "field.hpp"
#include "moves/moveBase.hpp"
#include "objectManager.hpp"
#include "src/animation/loopingAnimation.hpp"
#include "src/animation/linearAnimation.hpp"
#include "src/animation/fadeAnimation.hpp"
#include "src/collision/collManager.hpp"

class MoveBase;
class Field;

class Piece: public Drawable{
    public:
    Piece(Board* board, int objectID, Field* field);
    virtual void init() override;
    virtual void draw(glm::mat4 projection_matrix) override;
    virtual void drawShadow(mat4 projection_matrix) override;

    virtual void update(float elapsedTimeMs) override;
    virtual void MouseClick(vec3 position) override;

    void recreateGeoemtry() override;
    virtual vec3 Position3D() override;
    vec3 Position();
    Collision* BoundingBox;
    virtual bool isWhite(), isTransformable(), isCopyable();
    Field* _field = 0;
    Field* GetField();
    void SetField(Field* field, bool sim = false);
    virtual vector<MoveBase*> GetMoves() = 0;
    int GetIdWithoutColor();
    bool IsMoved = false;

    protected:
    virtual void UpdateBB(vec3 position);
    virtual std::string getVertexShader() override;
    virtual std::string getFragmentShader() override;
    void AddHitOrMove(Field* field, vector<MoveBase*> &moves);
    vec3 _position;
    float _yRotation;

    Board* _board;

    private:
    Geometry* _geo = 0;
};


#endif // PIECE_H
