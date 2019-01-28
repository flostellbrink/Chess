#ifndef SKYBOX_H
#define SKYBOX_H

#include "src/objects/Drawable.h"

class Geometry;

class Skybox : public Drawable
{
public:
  Skybox();

  void Init() override;
  void Draw(glm::mat4 projection_matrix) override;
  void DrawShadow(glm::mat4 projection_matrix) override;
  virtual void DrawSkybox(glm::mat4 projection_matrix);

  glm::vec3 Position3D() override;
  void Update(float elapsedTimeMs) override;
  void MouseClick(glm::vec3 position) override;

protected:
  std::string GetVertexShader() override;
  std::string GetFragmentShader() override;

private:
  Geometry* geometry_{};
};

#endif // SKYBOX_H
