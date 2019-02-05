#ifndef BASIC_OBJECT_H
#define BASIC_OBJECT_H

#include <glm/glm.hpp>
#include <string>
#include "Drawable.h"

class Geometry;
class Field;

class BasicObject : public Drawable
{
public:
  explicit BasicObject(int objectId,
                       glm::vec3 position = glm::vec3(),
                       float yRotation = 0,
                       std::string shader = "basic");
  void Init() override;
  void Draw(glm::mat4 projectionMatrix) override;
  void DrawShadow(glm::mat4 projectionMatrix) override;

  void Update(float elapsedTimeMs) override;
  void MouseClick() override;
  glm::vec3 Position() const;
  void Position(glm::vec3 position);
  glm::vec3 Position3D() override;
  void SetRotationX(float value);
  void SetRotationY(float value);
  void SetRotationZ(float value);

protected:
  std::string GetVertexShader() override;
  std::string GetFragmentShader() override;
  glm::vec3 position_;
  float y_rotation_;
  float x_rotation_ = 0.f;
  float z_rotation_ = 0.f;
  std::string shader_;

  Field* field_;
};


#endif // BASIC_OBJECT_H
