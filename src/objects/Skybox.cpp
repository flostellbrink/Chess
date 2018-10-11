#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "Skybox.h"
#include "ObjectManager.h"
#include "src/Objects.h"
#include "src/texture/Shader.h"
#include "src/geometry/Geometry.h"
#include "src/texture/Texture.h"


Skybox::Skybox() : Drawable(objects::skybox) {

}

void Skybox::Init() {
  Drawable::Init();
  geometry_ = ObjectManager::geometry.GetCube();
}

void Skybox::Draw(glm::mat4 projection_matrix) {
  (void)projection_matrix;
}

void Skybox::DrawShadow(glm::mat4 projection_matrix) {
  (void)projection_matrix;
}

void Skybox::DrawSkybox(glm::mat4 projection_matrix) {
  // Load program
  program_->Use();

  ObjectManager::texture.GetTexture(object_id_)[0]->Bind(GL_TEXTURE_CUBE_MAP);

  // set parameter
  program_->Bind(projection_matrix, "projection_matrix");
  program_->Bind(model_view_matrix_, "modelview_matrix");

  glDisable(GL_DEPTH_TEST);

  geometry_->Draw();

  glEnable(GL_DEPTH_TEST);
}

void Skybox::Update(float elapsedTimeMs) {
  (void)elapsedTimeMs;
}

void Skybox::MouseClick(glm::vec3 position) {
  (void)position;
}

std::string Skybox::GetVertexShader() {
  return "res/shader/Skybox.vs.glsl";
}

std::string Skybox::GetFragmentShader() {
  return "res/shader/Skybox.fs.glsl";
}

glm::vec3 Skybox::Position3D() {
  return glm::vec3();
}

void Skybox::RecreateGeometry() {
  // Box does not change
}
