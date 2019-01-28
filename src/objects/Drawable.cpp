#include <GL/glew.h>

#include "Drawable.h"
#include "src/texture/Image.h"
#include "src/objects/ObjectManager.h"
#include "src/texture/Shader.h"

glm::vec3 Drawable::light_pos = glm::vec3(0, 20, -5);
glm::vec3 Drawable::cam_pos = glm::vec3();
GLuint Drawable::reflection_texture;
GLuint Drawable::post_texture;
GLuint Drawable::shadow_texture;
glm::mat4 Drawable::shadow_view_projection;
int Drawable::overlay_state = 0;
float Drawable::overlay_opacity = 1.f;

Drawable::Drawable(const int objectId) :
  model_view_matrix_(1.0f), object_id_(objectId) {
}

void Drawable::Init() {
  InitShader();
}

void Drawable::InitShader() {
  program_ = new Shader(GetVertexShader(), GetFragmentShader());
  program_shadow_ = new Shader("res/shader/Shadow.vs.glsl", "res/shader/Shadow.fs.glsl");
}

void Drawable::DrawOpaque(glm::mat4 projection_matrix) {
  if (!ObjectManager::texture.IsTranslucent(object_id_)) {
    Draw(projection_matrix);
  }
}

void Drawable::DrawTranslucent(glm::mat4 projection_matrix) {
  if (ObjectManager::texture.IsTranslucent(object_id_)) {
    Draw(projection_matrix);
  }
}

void Drawable::DrawReflection(glm::mat4 projection_matrix) {
  // Ignores if translucent or not, because only translucent or only opaque objects are drawn
  if (TextureManager::IsReflected(object_id_)) {
    Draw(projection_matrix);
  }
}
