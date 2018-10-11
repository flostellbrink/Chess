#ifndef FULLSCREENQUAD_H
#define FULLSCREENQUAD_H

#include "Drawable.h"

class Geometry;

class FullScreenQuad : public Drawable {
public:
  FullScreenQuad(std::string vertexShader, std::string fragmentShader);
  void Init() override;
  void Draw(glm::mat4 projection_matrix) override;
  void DrawShadow(glm::mat4 projection_matrix) override;

  void Update(float elapsedTimeMs) override;
  void MouseClick(glm::vec3 position) override;
  glm::vec3 Position3D() override;
  void RecreateGeometry() override;

protected:
  std::string GetVertexShader() override;
  std::string GetFragmentShader() override;

  Geometry* geometry_ = nullptr;
  std::string vertex_shader_, fragment_shader_;
};


#endif // FULLSCREENQUAD_H
