#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader;

class Drawable {
public:
  virtual ~Drawable() = default;
  explicit Drawable(int objectId = 0);

  virtual void Init();
  virtual void Draw(glm::mat4 projectionMatrix) = 0;
  virtual void DrawOpaque(glm::mat4 projectionMatrix);
  virtual void DrawShadow(glm::mat4 projectionMatrix) = 0;
  virtual void DrawReflection(glm::mat4 projectionMatrix);
  virtual void DrawTranslucent(glm::mat4 projectionMatrix);

  virtual glm::vec3 Position3D() = 0;

  virtual void Update(float elapsedTimeMs) = 0;

  virtual void MouseClick(glm::vec3 position) = 0;

  static glm::vec3 light_pos;
  static glm::vec3 cam_pos;

  static GLuint reflection_texture, post_texture, shadow_texture;
  static glm::mat4 shadow_view_projection;
  static int overlay_state;
  static float overlay_opacity;

protected:

  Shader *program_{}, *program_shadow_{};
  GLuint vertex_array_object_ = 0;
  glm::mat4 model_view_matrix_;
  int object_id_;

  virtual void InitShader();
  virtual std::string GetVertexShader() = 0;
  virtual std::string GetFragmentShader() = 0;
};


#endif // DRAWABLE_H
