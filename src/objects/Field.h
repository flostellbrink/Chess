// class to represent a field on the board
#ifndef FIELD_H
#define FIELD_H

#include "Drawable.h"

class Board;
class Piece;
class Collision;
class Geometry;

class Field : public Drawable {
public:
  Field(Board* board, int boardX, int boardY, bool inactive);
  void Init() override;
  void Draw(glm::mat4 projectionMatrix) override;
  void DrawShadow(glm::mat4 projectionMatrix) override;

  void Update(float elapsedTimeMs) override;
  void MouseClick(glm::vec3 position) override;

  glm::vec3 Position3D() override;

  void EnableOverlay(bool enabled = true);
  void SetOverlayNumber(int overlay = 1);
  glm::vec3 TopPosition() const;
  glm::vec3 CenterPosition() const;
  int Row() const;
  int Column() const;
  Field *up = 0, *down = 0, *left = 0, *right = 0;
  Piece* current_piece = 0;

protected:
  std::string GetVertexShader() override;
  std::string GetFragmentShader() override;

  Collision* bounding_box_;

private:
  int board_x_, board_y_;
  Geometry* geometry_{};
  glm::vec3 position_;
  Board* board_;
  bool inactive_;
  bool overlay_enabled_ = false;
  int overlay_number_ = 1;
  float overlay_opacity_ = 0;
};


#endif // FIELD_H
