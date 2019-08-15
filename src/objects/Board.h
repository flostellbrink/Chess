#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <random>
#include <stack>
#include <tuple>

#include "src/collision/Collision.h"

class Piece;
class Field;
class Camera;
class MoveBase;

class Board
{
public:
  explicit Board();
  void NewTurn();
  void Update(float elapsedTime);
  void FieldClick(Field& field);
  void PieceClick(Piece& piece);
  std::shared_ptr<Field> GetSideField(bool whiteSide);
  void AddPiece(int objectId, Field& field);
  std::shared_ptr<MoveBase> GetLastMove();
  void UndoMove(bool sim = true);
  void EnableAi(bool enabled = true);
  bool GetAi() const;
  bool IsCastlingPossible(bool isWhite, bool isLeft);
  std::shared_ptr<Piece> GetKing(bool isWhite);
  std::shared_ptr<Piece> GetRook(bool isWhite, bool isLeft);
  std::shared_ptr<Field> GetField(int column, int row);
  bool IntersectsGame(const Collision& collision, const Piece* except = nullptr) const;
  void RunDemo();
  bool IsWhitesTurn() const;
  void SetState(int state);
  float overlay_rotation = 0.0f;
  float overlay_scale = 1.0f;

protected:
  const int
    fresh_ = 0,
    running_ = 1,
    white_won_ = 2,
    black_won_ = 3,
    draw_ = 4;
  int state_ = 0;

  void ClearOverlays();
  void SetOverlays();
  std::vector<std::shared_ptr<MoveBase>> GetValid(std::vector<std::shared_ptr<MoveBase>> moves);
  void UpdateCurrentMoves(std::vector<std::shared_ptr<MoveBase>> moves);
  bool ExistsValidMove();
  bool IsKingInMate();
  void DoAi();
  void ChangeTurn();

private:
  std::vector<std::vector<std::shared_ptr<Field>>> fields_;
  std::vector<std::vector<std::shared_ptr<Field>>> side_fields_;
  std::vector<std::shared_ptr<MoveBase>> current_moves_, current_invalid_moves_;
  std::stack<std::shared_ptr<MoveBase>> all_moves_;
  std::vector<std::shared_ptr<Piece>> pieces_;
  bool white_turn_ = false;
  bool locked_ = false;
  bool use_ai_ = false;
  bool ai_overdue_ = false;
  float ai_timer_ = 0.0f;
  glm::vec3 position_, size_;
  Collision bounding_box_;
  void ApplyAndPushMove(std::shared_ptr<MoveBase> move);

  std::default_random_engine generator_;
};


#endif // BOARD_H
