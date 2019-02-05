#ifndef FULL_SCREEN_QUAD_H
#define FULL_SCREEN_QUAD_H

#include "Drawable.h"

class Geometry;

class FullScreenQuad : public Drawable
{
public:
  FullScreenQuad(std::string vertexShader, std::string fragmentShader);
  void Init() override;
  void Draw(glm::mat4 projectionMatrix) override;
  void DrawShadow(glm::mat4 projectionMatrix) override;

  void Update(float elapsedTimeMs) override;
  void MouseClick() override;
  glm::vec3 Position3D() override;

protected:
  std::string GetVertexShader() override;
  std::string GetFragmentShader() override;

  std::string vertex_shader_, fragment_shader_;
};


#endif // FULL_SCREEN_QUAD_H
