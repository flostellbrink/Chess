// Abstract Class for all pieces
#ifndef PIECE_H
#define PIECE_H

#include "Drawable.h"
#include <vector>

class Board;
class Field;
class Collision;
class MoveBase;
class Geometry;

class Piece : public Drawable
{
public:
  Piece(Board* board, int objectId, Field* field);
  void Init() override;
  void Draw(glm::mat4 projectionMatrix) override;
  void DrawShadow(glm::mat4 projectionMatrix) override;

  void Update(float elapsedTimeMs) override;
  void MouseClick(glm::vec3 position) override;

  glm::vec3 Position3D() override;
  glm::vec3 Position() const;
  Collision* bounding_box;
  virtual bool IsWhite(), IsTransformable(), IsCopyable();
  Field* field = nullptr;
  Field* GetField() const;
  void SetField(Field* field, bool sim = false);
  virtual std::vector<MoveBase*> GetMoves() = 0;
  int GetIdWithoutColor();
  bool is_moved = false;

protected:
  virtual void UpdateBb(glm::vec3 position);
  std::string GetVertexShader() override;
  std::string GetFragmentShader() override;
  void AddHitOrMove(Field* field, std::vector<MoveBase*>& moves);

  glm::vec3 position_;
  float y_rotation_;
  Board* board_;

private:
  Geometry* geometry_ = nullptr;
};


#endif // PIECE_H
