// class to represent a field on the board
#ifndef FIELD_H
#define FIELD_H

#include "Drawable.h"
#include "src/collision/Collision.h"

class Board;
class Piece;
class Geometry;

class Field : public Drawable
{
public:
  Field(Board* board, int boardX, int boardY, bool inactive);
  void Init() override;
  void Draw(glm::mat4 projectionMatrix) override;
  void DrawShadow(glm::mat4 projectionMatrix) override;

  void Update(float elapsedTimeMs) override;
  void MouseClick() override;

  glm::vec3 Position3D() override;

  void EnableOverlay(bool enabled = true);
  void SetOverlayNumber(int overlay = 1);
  glm::vec3 TopPosition() const;
  glm::vec3 CenterPosition() const;
  int Row() const;
  int Column() const;
  Field *up = nullptr, *down = nullptr, *left = nullptr, *right = nullptr;
  Piece* current_piece = nullptr;

protected:
  std::string GetVertexShader() override;
  std::string GetFragmentShader() override;

private:
  int board_x_, board_y_;
  glm::vec3 position_, size_;
  Board* board_;
  bool inactive_;
  bool overlay_enabled_ = false;
  int overlay_number_ = 1;
  float overlay_opacity_ = 0;
  Collision bounding_box_;
};


#endif // FIELD_H
