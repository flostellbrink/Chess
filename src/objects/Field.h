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
  Field(Board* board_, int boardX, int boardY, bool inactive);
  void Init() override;
  void Draw(glm::mat4 projection_matrix) override;
  void DrawShadow(glm::mat4 projection_matrix) override;

  void Update(float elapsedTimeMs) override;
  void MouseClick(glm::vec3 position) override;

  void RecreateGeometry() override;

  glm::vec3 Position3D() override;

  void EnableOverlay(bool enabled = true);
  void SetOverlayNumber(int overlay = 1);
  glm::vec3 TopPosition() const;
  glm::vec3 CenterPosition() const;
  int Row() const;
  int Column() const;
  Field *Up = 0, *Down = 0, *Left = 0, *Right = 0;
  Piece* CurrentPiece = 0;

protected:
  std::string GetVertexShader() override;
  std::string GetFragmentShader() override;

  Collision* boundingBox_;

private:
  int boardX_, boardY_;
  Geometry* geometry_{};
  glm::vec3 position_;
  Board* board_;
  bool inactive_;
  bool overlayEnabled_ = false;
  int overlayNumber_ = 1;
  float ovelayOpacity_ = 0;
};


#endif // FIELD_H
