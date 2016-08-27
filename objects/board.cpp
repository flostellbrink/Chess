#include <GL/glew.h>

#include "board.hpp"
#include <string>

#include "math.h"
#ifndef M_PI
#define M_PI glm::pi<float>()
#endif

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <geometry/geoManager.hpp>
#include <iostream>
#include "glbase/gltool.hpp"
#include "objects.hpp"
#include "objectManager.hpp"
#include "moves/moveBase.hpp"
#include "moves/move.hpp"
#include "moves/hit.hpp"
#include "moves/rochade.hpp"
#include "moves/transform.hpp"

#include "pieces/pawn.hpp"
#include "pieces/bishop.hpp"
#include "pieces/knight.hpp"
#include "pieces/queen.hpp"
#include "pieces/king.hpp"
#include "pieces/rook.hpp"

using std::string;
using glm::vec3;
using std::tuple;

const int boardSize = 8;

Board::Board(Camera* camera) {
    _camera = camera;
    ObjectManager* mngr = &ObjectManager::Instance;
    vec3 size = vec3(8.5,1,8.5), center = vec3(0,-1,0);
    BoundingBox = ObjectManager::Collisions.GetAABB(center - size, center + size);
    // Create fields
    for (int i = 0; i < boardSize; ++i) {
        vector<Field*> row;
        for (int j = 0; j < boardSize; ++j){
            Field* field = new Field(this, i, j);
            row.push_back(field);
            mngr->AddObject(field);
        }
        _fields.push_back(row);
    }
    // Set the fields neighbouring fields, so we don't have to mess with indices later
    for (int i = 0; i < boardSize; ++i){
        for (int j = 0; j < boardSize; ++j) {
            if(i > 0)
                _fields[i][j]->Down = _fields[i-1][j];
            if(i < boardSize - 1)
                _fields[i][j]->Up = _fields[i+1][j];
            if(j > 0)
                _fields[i][j]->Left = _fields[i][j-1];
            if(j < boardSize - 1)
                _fields[i][j]->Right = _fields[i][j+1];
        }
    }
    // Create side Fields for captured pieces
    for (int i = 0; i < 2; ++i) {
        vector<Field*> row;
        for (int j = 0; j < boardSize * 3; ++j) {
            Field* field = new Field(this, i ? (j % boardSize) : boardSize - 1 - (j % boardSize), i ? boardSize - 1 + 2 + (j / boardSize) : - 2 - (j / boardSize), true);
            row.push_back(field);
            mngr->AddObject(field);
        }
        _sideFields.push_back(row);
    }
    // Create Pieces
    for (int color = 0; color < 2; ++color) {
        int row1 = color ? boardSize - 1 : 0, row2 = color ? boardSize - 2 : 1;
        AddPiece(objects::whiteRook + color, _fields[row1][0]);
        AddPiece(objects::whiteKnight + color, _fields[row1][1]);
        AddPiece(objects::whiteBishop + color, _fields[row1][2]);
        AddPiece(objects::whiteQueen + color, _fields[row1][3]);
        AddPiece(objects::whiteKing + color, _fields[row1][4]);
        AddPiece(objects::whiteBishop + color, _fields[row1][5]);
        AddPiece(objects::whiteKnight + color, _fields[row1][6]);
        AddPiece(objects::whiteRook + color, _fields[row1][7]);

        for (int i = 0; i < boardSize; ++i)
            AddPiece(objects::whitePawn + color, _fields[row2][i]);
    }
    // Add them to the manager
    for(Piece* piece : _pieces)
        mngr->AddObject(piece);
    // Create boards border
    for (int i = 0; i < 4; ++i) {
        mngr->AddObject(new BasicObject(objects::BoardBorderTop, vec3(), M_PI / 2. * (float) i, "mirror"));
        mngr->AddObject(new BasicObject(objects::BoardBorderBottom, vec3(), M_PI / 2. * (float) i, "texFromWorld"));
    }

    Drawable::OverlayState = -1;
    ObjectManager::Animations.PlayNow(new LinearAnimation<float>(15000, Drawable::OverlayOpacity, 1.f, 1.f));
    ObjectManager::Animations.PlayLast(new FadeAnimation<float>(5000, Drawable::OverlayOpacity ,Drawable::OverlayOpacity , 0.f));

    NewTurn();
}

void Board::ResetGame(){
    Drawable::OverlayState = -1;
    // On a proper reset this has to be animated
    //Drawable::OverlayOpacity = 0;
    ObjectManager::Instance.Animations.Reset();
    ObjectManager::Instance.NewGame();
}

void Board::AddPiece(unsigned int objectId, Field *field){
    Piece* piece = 0;
    switch (objectId) {
        case objects::whiteRook:
        case objects::blackRook:
            piece = new Rook(this, objectId, field);
            break;
        case objects::whiteKnight:
        case objects::blackKnight:
            piece = new Knight(this, objectId, field);
            break;
        case objects::whiteBishop:
        case objects::blackBishop:
            piece = new Bishop(this, objectId, field);
            break;
        case objects::whiteQueen:
        case objects::blackQueen:
            piece = new Queen(this, objectId, field);
            break;
        case objects::whiteKing:
        case objects::blackKing:
            piece = new King(this, objectId, field);
            break;
        case objects::whitePawn:
        case objects::blackPawn:
            piece = new Pawn(this, objectId, field);
            break;
    }
    if(piece){
        _pieces.push_back(piece);
        ObjectManager::Instance.AddObject(piece);
    }
}

void Board::NewTurn(){
    _whiteTurn = !_whiteTurn;
    if(!existsValidMove()){
        if(isKingInMate())
            setState(WhiteWon + _whiteTurn);
        else
            setState(Draw);
    } else {
        _whiteTurn = !_whiteTurn;
        changeTurn();
        doAI();
    }
}

void Board::setState(int state){
    if(state == _state) return;
    _state = state;
    Drawable::OverlayState = _state - 2;
    ObjectManager::Animations.PlayIndependent(new FadeAnimation<float>(1000, Drawable::OverlayOpacity, Drawable::OverlayOpacity, state > 1));
    switch(state){
        case 0:
            std::cout << "ChessInfo: State: Fresh" << std::endl;
            break;
        case 1:
            std::cout << "ChessInfo: State: Running" << std::endl;
            break;
        case 2:
            std::cout << "ChessInfo: State: White wins" << std::endl;
            break;
        case 3:
            std::cout << "ChessInfo: State: Black wins" << std::endl;
            break;
        case 4:
            std::cout << "ChessInfo: State: Draw" << std::endl;
            break;
    }
}

void Board::Update(){
    if(_aiOverdue)
        doAI();
}

void Board::changeTurn(){
    _whiteTurn = !_whiteTurn;
    _camera->SetBoardSide(_useAI ? true :_whiteTurn);
    std::cout << "ChessInfo: " << (_whiteTurn ? "Whites" : "Blacks") << " turn" << std::endl;
}

bool Board::isWhitesTurn() {
    return _whiteTurn;
}

void Board::doAI(){
    if(_state != Running) return;
    if(_whiteTurn || !_useAI || ObjectManager::Instance.Animations.isBusy()){
        _aiOverdue = true;
        return;
    }
    _aiOverdue = false;
    vector<Piece*> validPieces;
    for(Piece* piece : _pieces)
        if(piece->isWhite() == _whiteTurn && getValid(piece->GetMoves()).size())
            validPieces.push_back(piece);
    std::uniform_int_distribution<int> distribution(0,validPieces.size()-1);
    Piece *piece = validPieces[distribution(generator)];
    PieceClick(piece);
    std::uniform_int_distribution<int> distribution2(0,_currentMoves.size()-1);
    MoveBase* move = _currentMoves[distribution2(generator)];
    FieldClick(move->ClickField);
}

Field* Board::GetSideField(bool whiteSide){
    vector<Field*> row = _sideFields[whiteSide];
    for(Field* field : row)
        if(!field->Piece)
            return field;
}

MoveBase* Board::GetLastMove(){
    if(_allMoves.empty())
        return 0;
    return _allMoves.top();
}

void Board::UndoMove(bool sim){
    if(_allMoves.empty() || ObjectManager::Instance.Animations.isBusy()) return;
    setState(Running);
    MoveBase* move = _allMoves.top();
    move->Undo(this);
    _allMoves.pop();
    clearOverlays();
    if(!move->ChangeTurn()){
        _allMoves.top()->Undo(this);
        _allMoves.pop();
    }
    if(!sim)
        changeTurn();
    if(_allMoves.empty())
        setState(Fresh);
}

void Board::PieceClick(Piece *piece){
    if(_state > Running) return;
    if(ObjectManager::Instance.Animations.isBusy()) return;
    if(!locked && piece->isWhite() == _whiteTurn) {
        clearOverlays();
        tuple<vector<MoveBase*>,vector<MoveBase*>> moves = getValidAndInvalid(piece->GetMoves());
        _currentMoves = std::get<0>(moves);
        _currentInvalidMoves = std::get<1>(moves);
        setOverlays();
    } else
        FieldClick(piece->_field);
}

void Board::EnableAI(bool enabled){
    _useAI = enabled;
    if(enabled)
        doAI();
    else
        _aiOverdue = false;
}

bool Board::getAI() {
    return _useAI;
}


void Board::FieldClick(Field *field){
    if(_state > Running) return;
    for(MoveBase* move : _currentMoves)
        if(move->ClickField == field){
            setState(Running);
            locked = false;
            _allMoves.push(move);
            move->Apply(this);
            clearOverlays();
            _currentMoves.clear();
            if(field->Piece && field->Piece->isTransformable() && field->Row() == (field->Piece->isWhite() ? 7 : 0)){
                std::cout << "ChessInfo: Pawn is being promoted" << std::endl;
                locked = true;
                for(Piece* piece : _pieces)
                    if(piece->isCopyable())
                        _currentMoves.push_back(new Transform(field->Piece, piece));
                setOverlays();
                return;
            }
            NewTurn();
            return;
        }
}

void Board::setOverlays(){
    for(MoveBase* move : _currentMoves){
        move->ClickField->SetOverlayNumber(move->FieldOverlay);
        move->ClickField->EnableOverlay();
    }
    for(MoveBase* move : _currentInvalidMoves){
        move->ClickField->SetOverlayNumber(_whiteTurn ? 3 : 4);
        move->ClickField->EnableOverlay();
    }
}

void Board::clearOverlays(){
    for(MoveBase* move : _currentMoves)
        move->ClickField->EnableOverlay(false);
    _currentMoves.clear();
    for(MoveBase* move : _currentInvalidMoves)
        move->ClickField->EnableOverlay(false);
    _currentInvalidMoves.clear();
}

bool Board::existsValidMove(){
    for(Piece* piece : _pieces)
        if(piece->isWhite() == _whiteTurn && getValid(piece->GetMoves()).size())
            return true;
    return false;
}

vector<MoveBase*> Board::getValid(vector<MoveBase*> moves){
    vector<MoveBase*> result;
    for(MoveBase* move : moves){
        _allMoves.push(move);
        move->Apply(this, true);
        if(!isKingInMate())
            result.push_back(move);
        move->Undo(this, true);
        _allMoves.pop();
    }
    return result;
}

tuple<vector<MoveBase*>,vector<MoveBase*>> Board::getValidAndInvalid(vector<MoveBase *> moves){
    vector<MoveBase*> result1, result2;
    for(MoveBase* move : moves){
        _allMoves.push(move);
        move->Apply(this, true);
        if(!isKingInMate())
            result1.push_back(move);
        else {
            result2.push_back(move);
        }
        move->Undo(this, true);
        _allMoves.pop();
    }
    return tuple<vector<MoveBase*>,vector<MoveBase*>>(result1, result2);
}

Piece* Board::GetKing(bool isWhite){
    int index = 4;
    if(!isWhite) index += boardSize * 2;
    return _pieces[index];
}

Piece* Board::GetRook(bool isWhite, bool isLeft){
    int index = isLeft ? 0 : 7;
    if(!isWhite) index += boardSize * 2;
    return _pieces[index];
}

bool Board::isKingInMate(){
    Piece* king = GetKing(_whiteTurn);
    Field* kingField = king->_field;
    for(Piece* piece : _pieces){
        if(piece->isWhite() != _whiteTurn){
            vector<MoveBase*> moves = piece->GetMoves();
            for(MoveBase* move : moves){
                _allMoves.push(move);
                move->Apply(this, true);
                if(kingField != king->_field){
                    move->Undo(this, true);
                    _allMoves.pop();
                    return true;
                }
                move->Undo(this, true);
                _allMoves.pop();
            }
        }
    }
    return false;
}

bool Board::intersectsGame(Collision* collider, Piece *except){
    // TODO intersect with board
    foreach (Piece* piece, _pieces)
        if(piece != except && piece->BoundingBox->Intersects(collider))
            return true;
    return collider->Intersects(BoundingBox);
}

bool Board::isRochadePossible(bool isWhite, bool isLeft){
    Piece* king = GetKing(isWhite);
    Piece* rook = GetRook(isWhite, isLeft);
    // Basic condition
    if(rook->IsMoved || king->IsMoved) return false;
    bool success = true;
    // Field next to king
    Field* stepOverField = isLeft ? king->_field->Left : king->_field->Right;
    if(!stepOverField || stepOverField->Piece) return false;
    Move stepOver(king, stepOverField);
    stepOver.Apply(this, true);
        if(isKingInMate()) success = false;
    stepOver.Undo(this, true);
    // King destination field
    Field* stepField = isLeft ? stepOverField->Left : stepOverField->Right;
    if(!stepField || stepField->Piece || !success) return false;
    Move step(king, stepField);
    step.Apply(this, true);
        if(isKingInMate())  success = false;
        Move rookStep(rook, stepOverField);
        rookStep.Apply(this, true);
            if(isKingInMate()) success = false;
        rookStep.Undo(this, true);
    step.Undo(this, true);
    // Field next to rook
    Field* rookStepField = isLeft ? rook->_field->Right : rook->_field->Left;
    if(!rookStepField || rookStepField->Piece) return false;
    return success;
}

Field* Board::GetField(int column, int row){
    return _fields[row - 1][column - 1];
}

void Board::applyAndPushMove(MoveBase *move){
    move->Apply(this);
    _allMoves.push(move);
}

void Board::RunDemo(){
    if(_state) return;
    setState(Running);
    NewTurn();
    int a = 1, b = 2, c = 3, d = 4, e = 5, f = 6, g = 7, h = 8;



    // http://www.chessgames.com/perl/chessgame?gid=1011478

    // 1. e4 d6
    applyAndPushMove(new Move(GetField(e,2)->Piece, GetField(e,4)));
    applyAndPushMove(new Move(GetField(d,7)->Piece, GetField(d,6)));
    // 2. d4 Nf6
    applyAndPushMove(new Move(GetField(d,2)->Piece, GetField(d,4)));
    applyAndPushMove(new Move(GetField(g,8)->Piece, GetField(f,6)));
    // 3. Nc3 g6
    applyAndPushMove(new Move(GetField(b,1)->Piece, GetField(c,3)));
    applyAndPushMove(new Move(GetField(g,7)->Piece, GetField(g,6)));
    // 4. Be3 Bg7
    applyAndPushMove(new Move(GetField(c,1)->Piece, GetField(e,3)));
    applyAndPushMove(new Move(GetField(f,8)->Piece, GetField(g,7)));
    // 5. Qd2 c6
    applyAndPushMove(new Move(GetField(d,1)->Piece, GetField(d,2)));
    applyAndPushMove(new Move(GetField(c,7)->Piece, GetField(c,6)));
    // 6. f3 b5
    applyAndPushMove(new Move(GetField(f,2)->Piece, GetField(f,3)));
    applyAndPushMove(new Move(GetField(b,7)->Piece, GetField(b,5)));
    // 7. Nge2 Nbd7
    applyAndPushMove(new Move(GetField(g,1)->Piece, GetField(e,2)));
    applyAndPushMove(new Move(GetField(b,8)->Piece, GetField(d,7)));
    // 8. Bh6 Bxh6
    applyAndPushMove(new Move(GetField(e,3)->Piece, GetField(h,6)));
    applyAndPushMove(new Hit(GetField(g,7)->Piece, GetField(h,6)->Piece));
    // 9. Qxh6 Bb7
    applyAndPushMove(new Hit(GetField(d,2)->Piece, GetField(h,6)->Piece));
    applyAndPushMove(new Move(GetField(c,8)->Piece, GetField(b,7)));
    // 10. a3 e5
    applyAndPushMove(new Move(GetField(a,2)->Piece, GetField(a,3)));
    applyAndPushMove(new Move(GetField(e,7)->Piece, GetField(e,5)));
    // 11. O-O-O Qe7
    applyAndPushMove(new Rochade(GetKing(true), GetRook(true, true), GetField(c,1), GetField(d,1)));
    applyAndPushMove(new Move(GetField(d,8)->Piece, GetField(e,7)));
    // 12. Kb1 a6
    applyAndPushMove(new Move(GetField(c,1)->Piece, GetField(b,1)));
    applyAndPushMove(new Move(GetField(a,7)->Piece, GetField(a,6)));
    // 13. Nc1 O-O-O
    applyAndPushMove(new Move(GetField(e,2)->Piece, GetField(c,1)));
    applyAndPushMove(new Rochade(GetKing(false), GetRook(false, true), GetField(c,8), GetField(d,8)));
    // 14. Nb3 exd4
    applyAndPushMove(new Move(GetField(c,1)->Piece, GetField(b,3)));
    applyAndPushMove(new Hit(GetField(e,5)->Piece, GetField(d,4)->Piece));
    // 15. Rxd4 c5
    applyAndPushMove(new Hit(GetField(d,1)->Piece, GetField(d,4)->Piece));
    applyAndPushMove(new Move(GetField(c,6)->Piece, GetField(c,5)));
    // 16. Rd1 Nb6
    applyAndPushMove(new Move(GetField(d,4)->Piece, GetField(d,1)));
    applyAndPushMove(new Move(GetField(d,7)->Piece, GetField(b,6)));
    // 17. g3 Kb8
    applyAndPushMove(new Move(GetField(g,2)->Piece, GetField(g,3)));
    applyAndPushMove(new Move(GetField(c,8)->Piece, GetField(b,8)));
    // 18. Na5 Ba8
    applyAndPushMove(new Move(GetField(b,3)->Piece, GetField(a,5)));
    applyAndPushMove(new Move(GetField(b,7)->Piece, GetField(a,8)));
    // 19. Bh3 d5
    applyAndPushMove(new Move(GetField(f,1)->Piece, GetField(h,3)));
    applyAndPushMove(new Move(GetField(d,6)->Piece, GetField(d,5)));
    // 20. Qf4+ Ka7
    applyAndPushMove(new Move(GetField(h,6)->Piece, GetField(f,4)));
    applyAndPushMove(new Move(GetField(b,8)->Piece, GetField(a,7)));
    // 21. Rhe1 d4
    applyAndPushMove(new Move(GetField(h,1)->Piece, GetField(e,1)));
    applyAndPushMove(new Move(GetField(d,5)->Piece, GetField(d,4)));
    // 22. Nd5 Nbxd5
    applyAndPushMove(new Move(GetField(c,3)->Piece, GetField(d,5)));
    applyAndPushMove(new Hit(GetField(b,6)->Piece, GetField(d,5)->Piece));
    // 23. exd5 Qd6
    applyAndPushMove(new Hit(GetField(e,4)->Piece, GetField(d,5)->Piece));
    applyAndPushMove(new Move(GetField(e,7)->Piece, GetField(d,6)));
    // 24. Rxd4 cxd4
    applyAndPushMove(new Hit(GetField(d,1)->Piece, GetField(d,4)->Piece));
    applyAndPushMove(new Hit(GetField(c,5)->Piece, GetField(d,4)->Piece));
    // 25. Re7+ Kb6
    applyAndPushMove(new Move(GetField(e,1)->Piece, GetField(e,7)));
    applyAndPushMove(new Move(GetField(a,7)->Piece, GetField(b,6)));
    // 26. Qxd4+ Kxa5
    applyAndPushMove(new Hit(GetField(f,4)->Piece, GetField(d,4)->Piece));
    applyAndPushMove(new Hit(GetField(b,6)->Piece, GetField(a,5)->Piece));
    // 27. b4+ Ka4
    applyAndPushMove(new Move(GetField(b,2)->Piece, GetField(b,4)));
    applyAndPushMove(new Move(GetField(a,5)->Piece, GetField(a,4)));
    // 28. Qc3 Qxd5
    applyAndPushMove(new Move(GetField(d,4)->Piece, GetField(c,3)));
    applyAndPushMove(new Hit(GetField(d,6)->Piece, GetField(d,5)->Piece));
    // 29. Ra7 Bb7
    applyAndPushMove(new Move(GetField(e,7)->Piece, GetField(a,7)));
    applyAndPushMove(new Move(GetField(a,8)->Piece, GetField(b,7)));
    // 30. Rxb7 Qc4
    applyAndPushMove(new Hit(GetField(a,7)->Piece, GetField(b,7)->Piece));
    applyAndPushMove(new Move(GetField(d,5)->Piece, GetField(c,4)));
    // 31. Qxf6 Kxa3
    applyAndPushMove(new Hit(GetField(c,3)->Piece, GetField(f,6)->Piece));
    applyAndPushMove(new Hit(GetField(a,4)->Piece, GetField(a,3)->Piece));
    // 32. Qxa6+ Kxb4
    applyAndPushMove(new Hit(GetField(f,6)->Piece, GetField(a,6)->Piece));
    applyAndPushMove(new Hit(GetField(a,3)->Piece, GetField(b,4)->Piece));
    // 33. c3+ Kxc3
    applyAndPushMove(new Move(GetField(c,2)->Piece, GetField(c,3)));
    applyAndPushMove(new Hit(GetField(b,4)->Piece, GetField(c,3)->Piece));
    // 34. Qa1+ Kd2
    applyAndPushMove(new Move(GetField(a,6)->Piece, GetField(a,1)));
    applyAndPushMove(new Move(GetField(c,3)->Piece, GetField(d,2)));
    // 35. Qb2+ Kd1
    applyAndPushMove(new Move(GetField(a,1)->Piece, GetField(b,2)));
    applyAndPushMove(new Move(GetField(d,2)->Piece, GetField(d,1)));
    // 36. Bf1 Rd2
    applyAndPushMove(new Move(GetField(h,3)->Piece, GetField(f,1)));
    applyAndPushMove(new Move(GetField(d,8)->Piece, GetField(d,2)));
    // 37. Rd7 Rxd7
    applyAndPushMove(new Move(GetField(b,7)->Piece, GetField(d,7)));
    applyAndPushMove(new Hit(GetField(d,2)->Piece, GetField(d,7)->Piece));
    // 38. Bxc4 bxc4
    applyAndPushMove(new Hit(GetField(f,1)->Piece, GetField(c,4)->Piece));
    applyAndPushMove(new Hit(GetField(b,5)->Piece, GetField(c,4)->Piece));
    // 39. Qxh8 Rd3
    applyAndPushMove(new Hit(GetField(b,2)->Piece, GetField(h,8)->Piece));
    applyAndPushMove(new Move(GetField(d,7)->Piece, GetField(d,3)));
    // 40. Qa8 c3
    applyAndPushMove(new Move(GetField(h,8)->Piece, GetField(a,8)));
    applyAndPushMove(new Move(GetField(c,4)->Piece, GetField(c,3)));
    // 41. Qa4+ Ke1
    applyAndPushMove(new Move(GetField(a,8)->Piece, GetField(a,4)));
    applyAndPushMove(new Move(GetField(d,1)->Piece, GetField(e,1)));
    // 42. f4 f5
    applyAndPushMove(new Move(GetField(f,3)->Piece, GetField(f,4)));
    applyAndPushMove(new Move(GetField(f,7)->Piece, GetField(f,5)));
    // 43. Kc1 Rd2
    applyAndPushMove(new Move(GetField(b,1)->Piece, GetField(c,1)));
    applyAndPushMove(new Move(GetField(d,3)->Piece, GetField(d,2)));
    // 44. Qa7 1-0
    applyAndPushMove(new Move(GetField(a,4)->Piece, GetField(a,7)));
}
