#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.inl>
#include <iostream>

#include "Board.h"
#include "src/Objects.h"
#include "ObjectManager.h"
#include "src/objects/moves/MoveBase.h"
#include "src/objects/moves/Move.h"
#include "src/objects/moves/Hit.h"
#include "src/objects/moves/Castling.h"
#include "src/objects/moves/Transform.h"
#include "src/objects/pieces/Pawn.h"
#include "src/objects/pieces/Bishop.h"
#include "src/objects/pieces/Knight.h"
#include "src/objects/pieces/Queen.h"
#include "src/objects/pieces/King.h"
#include "src/objects/pieces/Rook.h"
#include "BasicObject.h"
#include "src/animation/FadeAnimation.h"
#include "Field.h"
#include "src/collision/Collision.h"
#include "src/animation/LinearAnimation.h"
#include "src/animation/LoopingAnimation.h"
#include "src/animation/BackLoopingAnimation.h"
#include "src/animation/GroupAnimation.h"
#include "src/Config.h"
#include "src/Logger.h"
#include "src/animation/GroupAnimation.h"
#include "Clock.h"

const int board_size = 8;

Board::Board()
{
  auto manager = &ObjectManager::instance;
  const auto center = glm::vec3(0, -1, 0);
  const auto size = glm::vec3(8.5f, 1, 8.5f);
  bounding_box_ = CollisionManager::GetAabb(center - size, center + size);

  // Create fields
  for (auto i = 0; i < board_size; ++i)
  {
    std::vector<Field*> row;
    for (auto j = 0; j < board_size; ++j)
    {
      auto* field = new Field(this, i, j, false);
      row.push_back(field);
      manager->AddObject(field);
    }
    fields_.push_back(row);
  }

  // Set the fields neighboring fields, so we don't have to mess with indices later
  for (auto i = 0; i < board_size; ++i)
  {
    for (auto j = 0; j < board_size; ++j)
    {
      if (i > 0)
      {
        fields_[i][j]->down = fields_[i - 1][j];
      }

      if (i < board_size - 1)
      {
        fields_[i][j]->up = fields_[i + 1][j];
      }

      if (j > 0)
      {
        fields_[i][j]->left = fields_[i][j - 1];
      }

      if (j < board_size - 1)
      {
        fields_[i][j]->right = fields_[i][j + 1];
      }
    }
  }

  // Create side Fields for captured pieces
  for (auto i = 0; i < 2; ++i)
  {
    std::vector<Field*> row;
    for (auto j = 0; j < board_size * 3; ++j)
    {
      auto* field = new Field(this,
                              i ? j % board_size : board_size - 1 - j % board_size,
                              i ? board_size - 1 + 2 + j / board_size : -2 - j / board_size,
                              true);
      row.push_back(field);
      manager->AddObject(field);
    }
    side_fields_.push_back(row);
  }

  // Create Pieces
  for (auto color = 0; color < 2; ++color)
  {
    const auto row1 = color ? board_size - 1 : 0, row2 = color ? board_size - 2 : 1;
    AddPiece(objects::white_rook + color, fields_[row1][0]);
    AddPiece(objects::white_knight + color, fields_[row1][1]);
    AddPiece(objects::white_bishop + color, fields_[row1][2]);
    AddPiece(objects::white_queen + color, fields_[row1][3]);
    AddPiece(objects::white_king + color, fields_[row1][4]);
    AddPiece(objects::white_bishop + color, fields_[row1][5]);
    AddPiece(objects::white_knight + color, fields_[row1][6]);
    AddPiece(objects::white_rook + color, fields_[row1][7]);

    for (auto i = 0; i < board_size; ++i)
    {
      AddPiece(objects::white_pawn + color, fields_[row2][i]);
    }
  }

  // Add them to the manager
  for (auto piece : pieces_)
  {
    manager->AddObject(piece);
  }

  // Create boards border
  for (auto i = 0; i < 4; ++i)
  {
    manager->AddObject(new BasicObject(objects::board_border_top,
                                       glm::vec3(),
                                       glm::pi<float>() / 2.f * static_cast<float>(i),
                                       "Mirror"));
    manager->AddObject(new BasicObject(objects::board_border_bottom,
                                       glm::vec3(),
                                       glm::pi<float>() / 2.f * static_cast<float>(i),
                                       "TexFromWorld"));
  }

  // Add clock
  manager->AddObject(new Clock());

  // Start animations
  Drawable::overlay_state = -1;
  ObjectManager::animation.PlayIndependent(
    new FadeAnimation<float>(5000, Drawable::overlay_opacity, Drawable::overlay_opacity, 0.f));
  ObjectManager::animation.PlayIndependent(
    new BackLoopingAnimation<float>(new FadeAnimation<float>(500, overlay_scale, 1, 1.2f)));
  ObjectManager::animation.PlayIndependent(
    new LoopingAnimation<float>(new LinearAnimation<float>(5000, overlay_rotation, 0, 2 * glm::pi<float>())));

  NewTurn();
}

void Board::ResetGame()
{
  Drawable::overlay_state = -1;
  ObjectManager::animation.Reset();
  ObjectManager::instance.NewGame();
}

void Board::AddPiece(const int objectId, Field* field)
{
  Piece* piece = nullptr;
  switch (objectId)
  {
  case objects::white_rook:
  case objects::black_rook:
    piece = new Rook(this, objectId, field);
    break;
  case objects::white_knight:
  case objects::black_knight:
    piece = new Knight(this, objectId, field);
    break;
  case objects::white_bishop:
  case objects::black_bishop:
    piece = new Bishop(this, objectId, field);
    break;
  case objects::white_queen:
  case objects::black_queen:
    piece = new Queen(this, objectId, field);
    break;
  case objects::white_king:
  case objects::black_king:
    piece = new King(this, objectId, field);
    break;
  case objects::white_pawn:
  case objects::black_pawn:
    piece = new Pawn(this, objectId, field);
    break;
  default: break;
  }
  if (piece)
  {
    pieces_.push_back(piece);
    ObjectManager::instance.AddObject(piece);
  }
}

void Board::NewTurn()
{
  white_turn_ = !white_turn_;
  if (!ExistsValidMove())
  {
    if (IsKingInMate())
      SetState(white_won_ + white_turn_);
    else
      SetState(draw_);
  }
  else
  {
    white_turn_ = !white_turn_;
    ChangeTurn();
    DoAi();
  }
}

void Board::SetState(const int state)
{
  if (state == state_) return;
  state_ = state;
  Drawable::overlay_state = state_ - 2;
  ObjectManager::animation.PlayIndependent(
    new FadeAnimation<float>(1000, Drawable::overlay_opacity, Drawable::overlay_opacity, state > 1));
  switch (state)
  {
  case 0:
    Logger::Info("State: Fresh");
    break;
  case 1:
    Logger::Info("State: Running");
    break;
  case 2:
    Logger::Info("State: White wins");
    break;
  case 3:
    Logger::Info("State: Black wins");
    break;
  case 4:
    Logger::Info("State: Draw");
    break;
  default: break;
  }
}

void Board::Update(const float elapsedTime)
{
  if (ai_overdue_)
  {
    ai_timer_ += elapsedTime;
    DoAi();
  }
}

void Board::ChangeTurn()
{
  white_turn_ = !white_turn_;
  Logger::Info((white_turn_ ? "Whites turn" : "Blacks turn"));
}

bool Board::IsWhitesTurn() const
{
  return white_turn_;
}

void Board::DoAi()
{
  if (state_ != running_) return;
  if (white_turn_ || !use_ai_ || ObjectManager::animation.IsBusy())
  {
    ai_overdue_ = true;
    ai_timer_ = 0.0f;
    return;
  }

  // Check that enough time has passed to click a piece
  if (ai_timer_ < Config::ai_click_delay)
  {
    return;
  }

  // Click a piece if none is selected
  if (current_moves_.empty())
  {
    std::vector<Piece*> validPieces;
    for (auto piece : pieces_)
    {
      if (piece->IsWhite() == white_turn_ && !GetValid(piece->GetMoves()).empty())
      {
        validPieces.push_back(piece);
      }
    }

    const auto pieceIndex = std::uniform_int_distribution<int>(0, static_cast<int>(validPieces.size()) - 1)(generator_);
    PieceClick(validPieces[pieceIndex]);
  }

  // Check that enough time has passed to make a move
  if (ai_timer_ < Config::ai_click_delay + Config::ai_move_delay)
  {
    return;
  }

  // Make a move until no moves are left (promotions)
  while (!current_moves_.empty())
  {
    const auto moveIndex = std::uniform_int_distribution<int
    >(0, static_cast<int>(current_moves_.size()) - 1)(generator_);
    FieldClick(current_moves_[moveIndex]->click_field);
  }

  // Reset timer
  ai_overdue_ = false;
  ai_timer_ = 0.0f;
}

Field* Board::GetSideField(const bool whiteSide)
{
  auto row = side_fields_[whiteSide];
  for (auto field : row)
  {
    if (!field->current_piece)
    {
      return field;
    }
  }

  Logger::Error("No empty side field found");
  return nullptr;
}

MoveBase* Board::GetLastMove()
{
  if (all_moves_.empty())
  {
    return nullptr;
  }
  return all_moves_.top();
}

void Board::UndoMove(const bool sim)
{
  if (all_moves_.empty() || ObjectManager::animation.IsBusy()) return;
  SetState(running_);
  auto move = all_moves_.top();
  move->Undo(this, sim);
  all_moves_.pop();
  ClearOverlays();
  if (!move->ChangeTurn())
  {
    all_moves_.top()->Undo(this, sim);
    all_moves_.pop();
  }

  if (!sim)
  {
    ChangeTurn();
  }

  if (all_moves_.empty())
  {
    SetState(fresh_);
  }
}

void Board::PieceClick(Piece* piece)
{
  if (state_ > running_)
  {
    return;
  }

  if (ObjectManager::animation.IsBusy())
  {
    return;
  }

  if (!locked_ && piece->IsWhite() == white_turn_)
  {
    ClearOverlays();
    auto moves = GetValidAndInvalid(piece->GetMoves());
    current_moves_ = std::get<0>(moves);
    current_invalid_moves_ = std::get<1>(moves);
    SetOverlays();
  }
  else
  {
    FieldClick(piece->field);
  }
}

void Board::EnableAi(const bool enabled)
{
  use_ai_ = enabled;
  if (enabled)
  {
    DoAi();
  }
  else
  {
    ai_overdue_ = false;
  }
}

bool Board::GetAi() const
{
  return use_ai_;
}

void Board::FieldClick(Field* field)
{
  if (state_ > running_) return;
  for (auto move : current_moves_)
    if (move->click_field == field)
    {
      SetState(running_);
      locked_ = false;
      all_moves_.push(move);
      move->Apply(this, false);
      ClearOverlays();
      current_moves_.clear();
      if (field->current_piece && field->current_piece->IsTransformable() && field->Row() == (
        field->current_piece->IsWhite() ? 7 : 0))
      {
        Logger::Info("Pawn is being promoted");
        locked_ = true;
        for (auto piece : pieces_)
        {
          if (piece->IsCopyable())
          {
            current_moves_.push_back(new Transform(field->current_piece, piece));
          }
        }
        SetOverlays();
        return;
      }
      NewTurn();
      return;
    }
}

void Board::SetOverlays()
{
  for (auto move : current_moves_)
  {
    move->click_field->SetOverlayNumber(move->field_overlay);
    move->click_field->EnableOverlay();
  }

  for (auto move : current_invalid_moves_)
  {
    move->click_field->SetOverlayNumber(white_turn_ ? 3 : 4);
    move->click_field->EnableOverlay();
  }
}

void Board::ClearOverlays()
{
  for (auto move : current_moves_)
  {
    move->click_field->EnableOverlay(false);
  }

  current_moves_.clear();
  for (auto move : current_invalid_moves_)
  {
    move->click_field->EnableOverlay(false);
  }

  current_invalid_moves_.clear();
}

bool Board::ExistsValidMove()
{
  for (auto piece : pieces_)
  {
    if (piece->IsWhite() == white_turn_ && !GetValid(piece->GetMoves()).empty())
    {
      return true;
    }
  }

  return false;
}

std::vector<MoveBase*> Board::GetValid(std::vector<MoveBase*> moves)
{
  std::vector<MoveBase*> result;
  for (auto move : moves)
  {
    all_moves_.push(move);
    move->Apply(this, true);
    if (!IsKingInMate())
    {
      result.push_back(move);
    }

    move->Undo(this, true);
    all_moves_.pop();
  }

  return result;
}

std::tuple<std::vector<MoveBase*>, std::vector<MoveBase*>> Board::GetValidAndInvalid(std::vector<MoveBase *> moves)
{
  std::vector<MoveBase*> result1, result2;
  for (auto move : moves)
  {
    all_moves_.push(move);
    move->Apply(this, true);
    if (!IsKingInMate())
      result1.push_back(move);
    else
    {
      result2.push_back(move);
    }

    move->Undo(this, true);
    all_moves_.pop();
  }
  return std::tuple<std::vector<MoveBase*>, std::vector<MoveBase*>>(result1, result2);
}

Piece* Board::GetKing(const bool isWhite)
{
  auto index = 4;
  if (!isWhite)
  {
    index += board_size * 2;
  }

  return pieces_[index];
}

Piece* Board::GetRook(const bool isWhite, const bool isLeft)
{
  auto index = isLeft ? 0 : 7;
  if (!isWhite)
  {
    index += board_size * 2;
  }

  return pieces_[index];
}

bool Board::IsKingInMate()
{
  const auto king = GetKing(white_turn_);
  const auto kingField = king->field;
  for (auto piece : pieces_)
  {
    if (piece->IsWhite() != white_turn_)
    {
      auto moves = piece->GetMoves();
      for (auto move : moves)
      {
        all_moves_.push(move);
        move->Apply(this, true);
        if (kingField != king->field)
        {
          move->Undo(this, true);
          all_moves_.pop();
          return true;
        }

        move->Undo(this, true);
        all_moves_.pop();
      }
    }
  }
  return false;
}

bool Board::IntersectsGame(Collision* collision, Piece* except)
{
  for (auto piece : pieces_)
  {
    if (piece != except && piece->bounding_box->Intersects(collision))
    {
      return true;
    }
  }

  return collision->Intersects(bounding_box_);
}

bool Board::IsCastlingPossible(const bool isWhite, const bool isLeft)
{
  auto king = GetKing(isWhite);
  auto rook = GetRook(isWhite, isLeft);

  // Basic condition
  if (rook->is_moved || king->is_moved) return false;
  auto success = true;

  // Field next to king
  auto stepOverField = isLeft ? king->field->left : king->field->right;
  if (!stepOverField || stepOverField->current_piece) return false;
  Move stepOver(king, stepOverField);
  stepOver.Apply(this, true);
  if (IsKingInMate()) success = false;
  stepOver.Undo(this, true);

  // King destination field
  const auto stepField = isLeft ? stepOverField->left : stepOverField->right;
  if (!stepField || stepField->current_piece || !success) return false;
  Move step(king, stepField);
  step.Apply(this, true);
  if (IsKingInMate()) success = false;
  Move rookStep(rook, stepOverField);
  rookStep.Apply(this, true);
  if (IsKingInMate()) success = false;
  rookStep.Undo(this, true);
  step.Undo(this, true);

  // Field next to rook
  const auto rookStepField = isLeft ? rook->field->right : rook->field->left;
  if (!rookStepField || rookStepField->current_piece) return false;
  return success;
}

Field* Board::GetField(const int column, const int row)
{
  return fields_[row - 1][column - 1];
}

void Board::ApplyAndPushMove(MoveBase* move)
{
  move->Apply(this, false);
  all_moves_.push(move);
}

void Board::RunDemo()
{
  if (state_) return;
  SetState(running_);
  NewTurn();
  const auto a = 1, b = 2, c = 3, d = 4, e = 5, f = 6, g = 7, h = 8;

  // http://www.chessgames.com/perl/chessgame?gid=1011478

  // 1. e4 d6
  ApplyAndPushMove(new Move(GetField(e, 2)->current_piece, GetField(e, 4)));
  ApplyAndPushMove(new Move(GetField(d, 7)->current_piece, GetField(d, 6)));
  // 2. d4 Nf6
  ApplyAndPushMove(new Move(GetField(d, 2)->current_piece, GetField(d, 4)));
  ApplyAndPushMove(new Move(GetField(g, 8)->current_piece, GetField(f, 6)));
  // 3. Nc3 g6
  ApplyAndPushMove(new Move(GetField(b, 1)->current_piece, GetField(c, 3)));
  ApplyAndPushMove(new Move(GetField(g, 7)->current_piece, GetField(g, 6)));
  // 4. Be3 Bg7
  ApplyAndPushMove(new Move(GetField(c, 1)->current_piece, GetField(e, 3)));
  ApplyAndPushMove(new Move(GetField(f, 8)->current_piece, GetField(g, 7)));
  // 5. Qd2 c6
  ApplyAndPushMove(new Move(GetField(d, 1)->current_piece, GetField(d, 2)));
  ApplyAndPushMove(new Move(GetField(c, 7)->current_piece, GetField(c, 6)));
  // 6. f3 b5
  ApplyAndPushMove(new Move(GetField(f, 2)->current_piece, GetField(f, 3)));
  ApplyAndPushMove(new Move(GetField(b, 7)->current_piece, GetField(b, 5)));
  // 7. Nge2 Nbd7
  ApplyAndPushMove(new Move(GetField(g, 1)->current_piece, GetField(e, 2)));
  ApplyAndPushMove(new Move(GetField(b, 8)->current_piece, GetField(d, 7)));
  // 8. Bh6 Bxh6
  ApplyAndPushMove(new Move(GetField(e, 3)->current_piece, GetField(h, 6)));
  ApplyAndPushMove(new Hit(GetField(g, 7)->current_piece, GetField(h, 6)->current_piece));
  // 9. Qxh6 Bb7
  ApplyAndPushMove(new Hit(GetField(d, 2)->current_piece, GetField(h, 6)->current_piece));
  ApplyAndPushMove(new Move(GetField(c, 8)->current_piece, GetField(b, 7)));
  // 10. a3 e5
  ApplyAndPushMove(new Move(GetField(a, 2)->current_piece, GetField(a, 3)));
  ApplyAndPushMove(new Move(GetField(e, 7)->current_piece, GetField(e, 5)));
  // 11. O-O-O Qe7
  ApplyAndPushMove(new Castling(GetKing(true), GetRook(true, true), GetField(c, 1), GetField(d, 1)));
  ApplyAndPushMove(new Move(GetField(d, 8)->current_piece, GetField(e, 7)));
  // 12. Kb1 a6
  ApplyAndPushMove(new Move(GetField(c, 1)->current_piece, GetField(b, 1)));
  ApplyAndPushMove(new Move(GetField(a, 7)->current_piece, GetField(a, 6)));
  // 13. Nc1 O-O-O
  ApplyAndPushMove(new Move(GetField(e, 2)->current_piece, GetField(c, 1)));
  ApplyAndPushMove(new Castling(GetKing(false), GetRook(false, true), GetField(c, 8), GetField(d, 8)));
  // 14. Nb3 exd4
  ApplyAndPushMove(new Move(GetField(c, 1)->current_piece, GetField(b, 3)));
  ApplyAndPushMove(new Hit(GetField(e, 5)->current_piece, GetField(d, 4)->current_piece));
  // 15. Rxd4 c5
  ApplyAndPushMove(new Hit(GetField(d, 1)->current_piece, GetField(d, 4)->current_piece));
  ApplyAndPushMove(new Move(GetField(c, 6)->current_piece, GetField(c, 5)));
  // 16. Rd1 Nb6
  ApplyAndPushMove(new Move(GetField(d, 4)->current_piece, GetField(d, 1)));
  ApplyAndPushMove(new Move(GetField(d, 7)->current_piece, GetField(b, 6)));
  // 17. g3 Kb8
  ApplyAndPushMove(new Move(GetField(g, 2)->current_piece, GetField(g, 3)));
  ApplyAndPushMove(new Move(GetField(c, 8)->current_piece, GetField(b, 8)));
  // 18. Na5 Ba8
  ApplyAndPushMove(new Move(GetField(b, 3)->current_piece, GetField(a, 5)));
  ApplyAndPushMove(new Move(GetField(b, 7)->current_piece, GetField(a, 8)));
  // 19. Bh3 d5
  ApplyAndPushMove(new Move(GetField(f, 1)->current_piece, GetField(h, 3)));
  ApplyAndPushMove(new Move(GetField(d, 6)->current_piece, GetField(d, 5)));
  // 20. Qf4+ Ka7
  ApplyAndPushMove(new Move(GetField(h, 6)->current_piece, GetField(f, 4)));
  ApplyAndPushMove(new Move(GetField(b, 8)->current_piece, GetField(a, 7)));
  // 21. Rhe1 d4
  ApplyAndPushMove(new Move(GetField(h, 1)->current_piece, GetField(e, 1)));
  ApplyAndPushMove(new Move(GetField(d, 5)->current_piece, GetField(d, 4)));
  // 22. Nd5 Nbxd5
  ApplyAndPushMove(new Move(GetField(c, 3)->current_piece, GetField(d, 5)));
  ApplyAndPushMove(new Hit(GetField(b, 6)->current_piece, GetField(d, 5)->current_piece));
  // 23. exd5 Qd6
  ApplyAndPushMove(new Hit(GetField(e, 4)->current_piece, GetField(d, 5)->current_piece));
  ApplyAndPushMove(new Move(GetField(e, 7)->current_piece, GetField(d, 6)));
  // 24. Rxd4 cxd4
  ApplyAndPushMove(new Hit(GetField(d, 1)->current_piece, GetField(d, 4)->current_piece));
  ApplyAndPushMove(new Hit(GetField(c, 5)->current_piece, GetField(d, 4)->current_piece));
  // 25. Re7+ Kb6
  ApplyAndPushMove(new Move(GetField(e, 1)->current_piece, GetField(e, 7)));
  ApplyAndPushMove(new Move(GetField(a, 7)->current_piece, GetField(b, 6)));
  // 26. Qxd4+ Kxa5
  ApplyAndPushMove(new Hit(GetField(f, 4)->current_piece, GetField(d, 4)->current_piece));
  ApplyAndPushMove(new Hit(GetField(b, 6)->current_piece, GetField(a, 5)->current_piece));
  // 27. b4+ Ka4
  ApplyAndPushMove(new Move(GetField(b, 2)->current_piece, GetField(b, 4)));
  ApplyAndPushMove(new Move(GetField(a, 5)->current_piece, GetField(a, 4)));
  // 28. Qc3 Qxd5
  ApplyAndPushMove(new Move(GetField(d, 4)->current_piece, GetField(c, 3)));
  ApplyAndPushMove(new Hit(GetField(d, 6)->current_piece, GetField(d, 5)->current_piece));
  // 29. Ra7 Bb7
  ApplyAndPushMove(new Move(GetField(e, 7)->current_piece, GetField(a, 7)));
  ApplyAndPushMove(new Move(GetField(a, 8)->current_piece, GetField(b, 7)));
  // 30. Rxb7 Qc4
  ApplyAndPushMove(new Hit(GetField(a, 7)->current_piece, GetField(b, 7)->current_piece));
  ApplyAndPushMove(new Move(GetField(d, 5)->current_piece, GetField(c, 4)));
  // 31. Qxf6 Kxa3
  ApplyAndPushMove(new Hit(GetField(c, 3)->current_piece, GetField(f, 6)->current_piece));
  ApplyAndPushMove(new Hit(GetField(a, 4)->current_piece, GetField(a, 3)->current_piece));
  // 32. Qxa6+ Kxb4
  ApplyAndPushMove(new Hit(GetField(f, 6)->current_piece, GetField(a, 6)->current_piece));
  ApplyAndPushMove(new Hit(GetField(a, 3)->current_piece, GetField(b, 4)->current_piece));
  // 33. c3+ Kxc3
  ApplyAndPushMove(new Move(GetField(c, 2)->current_piece, GetField(c, 3)));
  ApplyAndPushMove(new Hit(GetField(b, 4)->current_piece, GetField(c, 3)->current_piece));
  // 34. Qa1+ Kd2
  ApplyAndPushMove(new Move(GetField(a, 6)->current_piece, GetField(a, 1)));
  ApplyAndPushMove(new Move(GetField(c, 3)->current_piece, GetField(d, 2)));
  // 35. Qb2+ Kd1
  ApplyAndPushMove(new Move(GetField(a, 1)->current_piece, GetField(b, 2)));
  ApplyAndPushMove(new Move(GetField(d, 2)->current_piece, GetField(d, 1)));
  // 36. Bf1 Rd2
  ApplyAndPushMove(new Move(GetField(h, 3)->current_piece, GetField(f, 1)));
  ApplyAndPushMove(new Move(GetField(d, 8)->current_piece, GetField(d, 2)));
  // 37. Rd7 Rxd7
  ApplyAndPushMove(new Move(GetField(b, 7)->current_piece, GetField(d, 7)));
  ApplyAndPushMove(new Hit(GetField(d, 2)->current_piece, GetField(d, 7)->current_piece));
  // 38. Bxc4 bxc4
  ApplyAndPushMove(new Hit(GetField(f, 1)->current_piece, GetField(c, 4)->current_piece));
  ApplyAndPushMove(new Hit(GetField(b, 5)->current_piece, GetField(c, 4)->current_piece));
  // 39. Qxh8 Rd3
  ApplyAndPushMove(new Hit(GetField(b, 2)->current_piece, GetField(h, 8)->current_piece));
  ApplyAndPushMove(new Move(GetField(d, 7)->current_piece, GetField(d, 3)));
  // 40. Qa8 c3
  ApplyAndPushMove(new Move(GetField(h, 8)->current_piece, GetField(a, 8)));
  ApplyAndPushMove(new Move(GetField(c, 4)->current_piece, GetField(c, 3)));
  // 41. Qa4+ Ke1
  ApplyAndPushMove(new Move(GetField(a, 8)->current_piece, GetField(a, 4)));
  ApplyAndPushMove(new Move(GetField(d, 1)->current_piece, GetField(e, 1)));
  // 42. f4 f5
  ApplyAndPushMove(new Move(GetField(f, 3)->current_piece, GetField(f, 4)));
  ApplyAndPushMove(new Move(GetField(f, 7)->current_piece, GetField(f, 5)));
  // 43. Kc1 Rd2
  ApplyAndPushMove(new Move(GetField(b, 1)->current_piece, GetField(c, 1)));
  ApplyAndPushMove(new Move(GetField(d, 3)->current_piece, GetField(d, 2)));
  // 44. Qa7 1-0
  ApplyAndPushMove(new Move(GetField(a, 4)->current_piece, GetField(a, 7)));
}
