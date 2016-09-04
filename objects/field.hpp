// class to represent a field on the board
#ifndef FIELD_H
#define FIELD_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include "drawable.hpp"
#include "piece.hpp"
#include "collision/collision.hpp"

using glm::vec3;

class Piece;
class Board;

class Field : public Drawable {
    public:
    Field(Board* _board, int boardX, int boardY, bool inactive = false);
    virtual void init() override;
    virtual void draw(glm::mat4 projection_matrix) override;
    virtual void drawShadow(mat4 projection_matrix) override;

    virtual void update(float elapsedTimeMs) override;
    virtual void MouseClick(vec3 position) override;

    void recreateGeoemtry() override;

    virtual vec3 Position3D() override;

    void EnableOverlay(bool enabled = true);
    void SetOverlayNumber(int overlay = 1);
    vec3 TopPosition();
    vec3 CenterPosition();
    int Row();
    int Column();
    Field *Up = 0, *Down = 0, *Left = 0, *Right = 0;
    Piece* CurrentPiece = 0;

    protected:
    virtual std::string getVertexShader() override;
    virtual std::string getFragmentShader() override;
    static float overlayRotation, overlayScale;

    Collision* _boundingBox;

    private:
    int _boardX, _boardY;
    Geometry* _geo;
    vec3 _position;
    Board* _board;
    bool _inactive;
    bool _overlayEnabled = false;
    int _overlayNumber = 1;
    float _ovelayOpacity = 0;

    friend class constructor;
       struct constructor {
           constructor();
       };
    static constructor cons;
};


#endif // FIELD_H
