#include <GL/glew.h>

#include "Drawable.h"
#include "src/objects/ObjectManager.h"
#include "src/texture/Shader.h"
#include "src/Logger.h"

glm::vec3 Drawable::light_pos = glm::vec3(0, 20, -20);
glm::vec3 Drawable::cam_pos = glm::vec3();
GLuint Drawable::reflection_texture;
GLuint Drawable::post_texture;
GLuint Drawable::shadow_texture;
glm::mat4 Drawable::shadow_view_projection;
int Drawable::overlay_state = 0;
float Drawable::overlay_opacity = 1.f;

Drawable::Drawable(const int objectId) :
  model_view_matrix_(1.0f),
  object_id_(objectId)
{
}

void Drawable::Init()
{
  InitShader();
}

void Drawable::DrawId(glm::mat4 projectionMatrix, int id)
{
  if (program_id_ == nullptr)
  {
    Logger::Error("program not loaded");
  }
  program_id_->Use();

  program_id_->Bind(projectionMatrix, "view_projection_matrix");
  program_id_->Bind(model_view_matrix_, "model_matrix");
  program_id_->Bind(id, "object_id");

  geometry_->Draw();
}

void Drawable::InitShader()
{
  program_ = Shader::GetShaderCached(GetVertexShader(), GetFragmentShader());
  program_shadow_ = Shader::GetShaderCached("res/shader/Shadow.vs.glsl", "res/shader/Shadow.fs.glsl");
  program_id_ = Shader::GetShaderCached("res/shader/Id.vs.glsl", "res/shader/Id.fs.glsl");
}

void Drawable::DrawOpaque(const glm::mat4 projectionMatrix)
{
  if (!ObjectManager::texture.IsTranslucent(object_id_))
  {
    Draw(projectionMatrix);
  }
}

void Drawable::DrawTranslucent(const glm::mat4 projectionMatrix)
{
  if (ObjectManager::texture.IsTranslucent(object_id_))
  {
    Draw(projectionMatrix);
  }
}

void Drawable::DrawReflection(const glm::mat4 projectionMatrix)
{
  // Ignores if translucent or not, because only translucent or only opaque objects are drawn
  if (TextureManager::IsReflected(object_id_))
  {
    Draw(projectionMatrix);
  }
}
