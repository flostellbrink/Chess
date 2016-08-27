// Abstract Class for all pieces
#ifndef PIECE_H
#define PIECE_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "drawable.hpp"
#include "field.hpp"
#include "animation/loopingAnimation.hpp"
#include "animation/linearAnimation.hpp"
#include "animation/fadeAnimation.hpp"
#include "moves/moveBase.hpp"
#include "objectManager.hpp"
#include "collision/collManager.hpp"

class MoveBase;
class Field;

class Piece: public Drawable{
    public:
    Piece(Board* board, int objectID, Field* field);
    virtual void init() override;
    virtual void draw(glm::mat4 projection_matrix) override;
    virtual void drawShadow(mat4 projection_matrix) override;
    virtual void drawSpecular(mat4 projection_matrix) override;
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
