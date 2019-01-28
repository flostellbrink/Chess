#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <random>
#include <stack>
#include <tuple>

class Collision;
class Piece;
class Field;
class Camera;
class MoveBase;

class Board {
public:
  explicit Board(Camera* camera);
  static void ResetGame();
  void NewTurn();
  void Update(float elapsedTime);
  void FieldClick(Field* field);
  void PieceClick(Piece* piece);
  Field* GetSideField(bool whiteSide);
  void AddPiece(int objectId, Field* field);
  MoveBase* GetLastMove();
  void UndoMove(bool sim = true);
  void EnableAi(bool enabled = true);
  bool GetAi() const;
  bool IsRochadePossible(bool isWhite, bool isLeft);
  Piece* GetKing(bool isWhite);
  Piece* GetRook(bool isWhite, bool isLeft);
  Field* GetField(int column, int row);
  bool IntersectsGame(Collision* collision, Piece* except = 0);
  void RunDemo();
  bool IsWhitesTurn() const;
  void SetState(int state);
  float overlay_rotation = 0.0f;
  float overlay_scale = 1.0f;

protected:
  const int
    Fresh = 0,
    Running = 1,
    WhiteWon = 2,
    BlackWon = 3,
    Draw = 4;
  int state_ = 0;

  void ClearOverlays();
  void SetOverlays();
  std::vector<MoveBase*> GetValid(std::vector<MoveBase*> moves);
  std::tuple<std::vector<MoveBase*>, std::vector<MoveBase*>> GetValidAndInvalid(std::vector<MoveBase*> moves);
  bool ExistsValidMove();
  bool IsKingInMate();
  void DoAi();
  void ChangeTurn();

private:
  std::vector<std::vector<Field*>> fields_;
  std::vector<std::vector<Field*>> sideFields_;
  std::vector<MoveBase*> currentMoves_, currentInvalidMoves_;
  std::vector<Piece*> pieces_;
  std::stack<MoveBase*> allMoves_;
  bool white_turn_ = false;
  bool locked_ = false;
  bool use_ai_ = false;
  bool ai_overdue_ = false;
  float ai_timer_ = 0.0f;
  Camera* camera_;
  Collision* bounding_box_;
  void ApplyAndPushMove(MoveBase* move);

  std::default_random_engine generator;
};


#endif // BOARD_H
