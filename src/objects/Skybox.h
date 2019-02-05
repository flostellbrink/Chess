#ifndef SKY_BOX_H
#define SKY_BOX_H

#include "src/objects/Drawable.h"

class Geometry;

class Skybox : public Drawable
{
public:
  Skybox();

  void Init() override;
  void Draw(glm::mat4 projectionMatrix) override;
  void DrawShadow(glm::mat4 projectionMatrix) override;
  virtual void DrawSkybox(glm::mat4 projectionMatrix);

  glm::vec3 Position3D() override;
  void Update(float elapsedTimeMs) override;
  void MouseClick() override;

protected:
  std::string GetVertexShader() override;
  std::string GetFragmentShader() override;
};

#endif // SKY_BOX_H
