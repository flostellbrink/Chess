#include <utility>

#include <GL/glew.h>

#include "FullScreenQuad.h"
#include <glm/glm.hpp>
#include "ObjectManager.h"
#include "src/Config.h"
#include "src/Objects.h"
#include "src/Logger.h"
#include "src/texture/Shader.h"
#include "src/geometry/Geometry.h"
#include "src/texture/Texture.h"


FullScreenQuad::FullScreenQuad(std::string vertexShader, std::string fragmentShader)
  : Drawable(objects::fullscreen_quad)
{
  vertex_shader_ = std::move(vertexShader);
  fragment_shader_ = std::move(fragmentShader);
}

void FullScreenQuad::Init()
{
  Drawable::Init();
  geometry_ = ObjectManager::geometry.GetGeometryCached(object_id_);
}

void FullScreenQuad::Draw(glm::mat4 projectionMatrix)
{
  (void)projectionMatrix;

  if (program_ == nullptr)
  {
    Logger::Error("program not loaded");
  }
  program_->Use();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, post_texture);
  program_->Bind(0, "tex");
  program_->Bind(overlay_opacity, "overlayOpacity");

  const glm::vec2 overlaySize(2.f, 1.f);
  const auto overlayRatio = overlaySize.x / overlaySize.y;
  const auto viewPortRatio = static_cast<float>(Config::viewport_width) / static_cast<float>(Config::viewport_height);
  auto fac = viewPortRatio < overlayRatio
               ? glm::vec2(1.f, overlayRatio / viewPortRatio)
               : glm::vec2(viewPortRatio / overlayRatio, 1.f);
  fac *= 1.5f;
  if (program_->HasUniform("overlayFac"))
  {
    program_->Bind(fac, "overlayFac");
  }

  // Make sure textures are loaded before needed, popping and frame drop are not nice!
  ObjectManager::texture.GetTexture(object_id_);
  if (program_->HasUniform("texOverlay"))
  {
    if (overlay_opacity > 0 && overlay_state >= 0)
    {
      ObjectManager::texture.GetTexture(object_id_)[overlay_state]->Bind(GL_TEXTURE_2D, 1);
      program_->Bind(1, "texOverlay");
      program_->Bind(1, "texOverlayEnabled");
    }
    else
    {
      program_->Bind(0, "texOverlayEnabled");
    }
  }

  geometry_->Draw();
}

void FullScreenQuad::DrawShadow(glm::mat4 projectionMatrix)
{
  (void)projectionMatrix;
}

void FullScreenQuad::Update(float elapsedTimeMs)
{
  (void)elapsedTimeMs;
}

void FullScreenQuad::MouseClick(glm::vec3 position)
{
  (void)position;
}

std::string FullScreenQuad::GetVertexShader()
{
  return vertex_shader_;
}

std::string FullScreenQuad::GetFragmentShader()
{
  return fragment_shader_;
}

glm::vec3 FullScreenQuad::Position3D()
{
  return glm::vec3();
}
