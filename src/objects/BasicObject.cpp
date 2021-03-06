#include <utility>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "BasicObject.h"
#include "src/Logger.h"
#include "src/animation/FadeAnimation.h"
#include "ObjectManager.h"
#include "src/texture/Shader.h"
#include "src/texture/Texture.h"
#include "src/geometry/Geometry.h"


BasicObject::BasicObject(const int objectId, const glm::vec3 position, const float yRotation, std::string shader)
  : Drawable(objectId), position_(position), y_rotation_(yRotation), shader_(std::move(shader)), field_(nullptr)
{
}

void BasicObject::Init()
{
  Drawable::Init();
  geometry_ = ObjectManager::geometry.GetGeometryCached(object_id_);
}

glm::vec3 BasicObject::Position() const
{
  return position_;
}

void BasicObject::Position(const glm::vec3 position)
{
  ObjectManager::animation.PlayIndependent(new FadeAnimation<glm::vec3>(500, position_, position_, position));
}

void BasicObject::Draw(glm::mat4 projectionMatrix)
{
  if (program_ == nullptr)
  {
    Logger::Error("program not loaded");
  }
  program_->Use();

  ObjectManager::texture.GetTexture(object_id_)[0]->Bind(GL_TEXTURE_2D);

  glActiveTexture(GL_TEXTURE0 + 2);
  glBindTexture(GL_TEXTURE_2D, reflection_texture);
  glActiveTexture(GL_TEXTURE0 + 3);
  glBindTexture(GL_TEXTURE_2D, shadow_texture);

  program_->Bind(0, "tex");
  if (program_->HasUniform("texShadow"))
  {
    program_->Bind(3, "texShadow");
  }

  /* TODO fix this hack
   * The mirror shader is a combination of mirror and overlay.
   * There isn't a separate mirror shader without overlay.
   * This means that we have to bind texOverlay to something.
   * Otherwise we would get a warning.
   */
  if (program_->HasUniform("texOverlay"))
  {
    program_->Bind(0, "texOverlay");
  }

  program_->Bind(projectionMatrix, "view_projection_matrix");
  if (program_->HasUniform("view_projection_shadow"))
  {
    auto viewProjectionShadow = shadow_view_projection * model_view_matrix_;
    program_->Bind(viewProjectionShadow, "view_projection_shadow");
  }

  program_->Bind(model_view_matrix_, "model_matrix");
  if (program_->HasUniform("tra_inv_model_matrix"))
  {
    auto traInvModelMatrix = transpose(inverse(model_view_matrix_));
    program_->Bind(traInvModelMatrix, "tra_inv_model_matrix");
  }

  if (program_->HasUniform("lightPos"))
  {
    program_->Bind(light_pos, "lightPos");
  }
  if (program_->HasUniform("camPos"))
  {
    program_->Bind(cam_pos, "camPos");
  }

  if (program_->HasUniform("La"))
  {
    auto la = glm::vec3(0.5f, 0.5f, 0.5f);
    program_->Bind(la, "La");
  }
  if (program_->HasUniform("ka"))
  {
    auto ka = glm::vec3(0.5f, 0.5f, 0.5f);
    program_->Bind(ka, "ka");
  }
  if (program_->HasUniform("Ld"))
  {
    auto ld = glm::vec3(0.5f, 0.5f, 0.5f);
    program_->Bind(ld, "Ld");
  }
  if (program_->HasUniform("kd"))
  {
    auto kd = glm::vec3(1, 1, 1);
    program_->Bind(kd, "kd");
  }

  if (program_->HasUniform("reflectivity"))
  {
    program_->Bind(ObjectManager::texture.Reflectivity(object_id_), "reflectivity");
  }
  if (program_->HasUniform("shininess"))
  {
    program_->Bind(ObjectManager::texture.Shininess(object_id_), "shininess");
  }

  geometry_->Draw();
}

void BasicObject::DrawShadow(const glm::mat4 projectionMatrix)
{
  program_shadow_->Use();

  auto viewProjectionShadow = projectionMatrix * model_view_matrix_;
  program_shadow_->Bind(viewProjectionShadow, "view_projection_shadow");

  geometry_->Draw();
}

void BasicObject::Update(float elapsedTimeMs)
{
  (void)elapsedTimeMs;

  model_view_matrix_ = translate(glm::mat4(1.0f), Position());
  model_view_matrix_ = rotate(model_view_matrix_, y_rotation_, glm::vec3(0, 1, 0));
  model_view_matrix_ = rotate(model_view_matrix_, x_rotation_, glm::vec3(1, 0, 0));
  model_view_matrix_ = rotate(model_view_matrix_, z_rotation_, glm::vec3(0, 0, 1));
}

void BasicObject::MouseClick()
{
}

std::string BasicObject::GetVertexShader()
{
  return "res/shader/" + shader_ + ".vs.glsl";
}

std::string BasicObject::GetFragmentShader()
{
  return "res/shader/" + shader_ + ".fs.glsl";
}

glm::vec3 BasicObject::Position3D()
{
  return Position();
}

void BasicObject::SetRotationX(const float value)
{
  x_rotation_ = value;
}

void BasicObject::SetRotationY(const float value)
{
  y_rotation_ = value;
}

void BasicObject::SetRotationZ(const float value)
{
  z_rotation_ = value;
}
