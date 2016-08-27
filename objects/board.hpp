#ifndef BOARD_H
#define BOARD_H

#include <GL/glew.h>
#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "drawable.hpp"
#include <vector>
#include <random>
#include <stack>
#include <tuple>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "collision/collision.hpp"
#include "geometry/geometry.hpp"
#include "geometry/geoManager.hpp"
#include "field.hpp"
#include "piece.hpp"
#include "clock.hpp"

using std::string;
using std::vector;
using std::stack;
using std::tuple;

class Piece;
class Field;
class MoveBase;
class Camera;
class Clock;

class Board {
public:
    Board(Camera* camera);
    void ResetGame();
    void NewTurn();
    void Update();
    void FieldClick(Field* field);
    void PieceClick(Piece* piece);
    Field* GetSideField(bool whiteSide);
    void AddPiece(unsigned int objectId, Field* field);
    MoveBase* GetLastMove();
    void UndoMove(bool sim = true);
    void EnableAI(bool enabled = true);
    bool getAI();
    bool isRochadePossible(bool isWhite, bool isLeft);
    Piece* GetKing(bool isWhite);
    Piece* GetRook(bool isWhite, bool isLeft);
    Field* GetField(int column, int row);
    bool intersectsGame(Collision* collider, Piece* except = 0);
    void RunDemo();
    bool isWhitesTurn();
    void setState(int state);

protected:
    const unsigned int
    Fresh = 0,
    Running = 1,
    WhiteWon = 2,
    BlackWon = 3,
    Draw = 4;
    int _state = 0;

    void clearOverlays();
    void setOverlays();
    vector<MoveBase*> getValid(vector<MoveBase*> moves);
    tuple<vector<MoveBase*>,vector<MoveBase*>> getValidAndInvalid(vector<MoveBase*> moves);
    bool existsValidMove();
    bool isKingInMate();
    void doAI();
    void changeTurn();

private:
    vector<vector<Field*>> _fields;
    vector<vector<Field*>> _sideFields;
    vector<MoveBase*> _currentMoves, _currentInvalidMoves;
    vector<Piece*> _pieces;
    stack<MoveBase*> _allMoves;
    bool _whiteTurn = false, locked = false, _useAI = false, _aiOverdue = false;
    Camera* _camera;
    Collision* BoundingBox;
    void applyAndPushMove(MoveBase* move);

    std::default_random_engine generator;
};


#endif // BOARD_H
