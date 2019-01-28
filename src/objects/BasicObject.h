#ifndef BASICOBJECT_H
#define BASICOBJECT_H

#include <glm/glm.hpp>
#include <string>
#include "Drawable.h"

class Geometry;
class Field;

class BasicObject : public Drawable {
public:
  explicit BasicObject(int objectID, glm::vec3 position = glm::vec3(), float yRotation = 0, std::string shader = "basic");
  void Init() override;
  void Draw(glm::mat4 projection_matrix) override;
  void DrawShadow(glm::mat4 projection_matrix) override;

  void Update(float elapsedTimeMs) override;
  void MouseClick(glm::vec3 position) override;
  glm::vec3 Position() const;
  void Position(glm::vec3 position);
  glm::vec3 Position3D() override;
  void SetRotationX(float val);
  void SetRotationY(float val);
  void SetRotationZ(float val);

protected:
  std::string GetVertexShader() override;
  std::string GetFragmentShader() override;
  glm::vec3 position_;
  float y_rotation_;
  float x_rotation_ = 0.f;
  float z_rotation_ = 0.f;
  std::string shader_;

  Geometry* geometry_ = 0;
  Field* field_;
};


#endif // BASICOBJECT_H
