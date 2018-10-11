#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <src/Logger.h>
#include <algorithm>

#include "Piece.h"
#include "ObjectManager.h"
#include "src/animation/FadeAnimation.h"
#include "src/animation/CatmullRomAnimation.h"
#include "src/objects/moves/Hit.h"
#include "src/objects/moves/Move.h"
#include "Field.h"
#include "src/geometry/Geometry.h"
#include "src/collision/Collision.h"
#include "Board.h"
#include "src/texture/Shader.h"
#include "src/texture/Texture.h"


Piece::Piece(Board* board, int objectID, Field* field)
  : Drawable(objectID), field(field), position_(field->TopPosition()), y_rotation_(0) {
  field->CurrentPiece = this;
  board_ = board;
  bounding_box = CollisionManager::GetAABB(glm::vec3(), glm::vec3());
  Piece::UpdateBb(Position());
}

void Piece::Init() {
  Drawable::Init();
  RecreateGeometry();
}

void Piece::RecreateGeometry() {
  delete geometry_;
  geometry_ = ObjectManager::geometry.GetGeometry(object_id_);
}

void Piece::UpdateBb(glm::vec3 position) {
  const auto size = glm::vec3(1.5f, 5, 1.5f);
  bounding_box->min = position - size * 0.5f;
  bounding_box->max = position + size * 0.5f;
}

glm::vec3 Piece::Position() const
{
  return position_;
}

Field* Piece::GetField() const
{
  return field;
}

void Piece::SetField(Field* field, const bool sim) {
  if (field == Piece::field) {
    field->CurrentPiece = this;
    return;
  }

  if (Piece::field->CurrentPiece == this) {
    Piece::field->CurrentPiece = nullptr;
  }

  if (!sim) {
    const auto from = Piece::field->TopPosition();
    const auto to = field->TopPosition();
    const auto direction = to - from;
    const auto up = glm::vec3(0, 1, 0);
    const float duration = 1000;
    UpdateBb(to);

    const auto directLine = CollisionManager::GetRay(from + up * 0.1f, to + up * 0.1f);
    if (!board_->IntersectsGame(directLine, this)) {
      const auto animation = new FadeAnimation<glm::vec3>(static_cast<int>(duration), position_, from, to);
      ObjectManager::animation.PlayLast(animation);
    }
    else {
      const auto maxY = std::max(from.y, to.y);
      auto p1 = from + direction * 0.1f;
      auto p2 = to - direction * 0.1f;
      p1.y = p2.y = maxY + 3;
      const auto animation = new CatmullRomAnimation<glm::vec3>(static_cast<int>(duration), position_, std::vector<glm::vec3>{from, p1, p2, to});
      ObjectManager::animation.PlayLast(animation);
    }
  }

  Piece::field = field;
  Piece::field->CurrentPiece = this;
}

bool Piece::IsWhite() {
  return !(object_id_ % 2);
}

bool Piece::IsTransformable() {
  return false;
}

bool Piece::IsCopyable() {
  return true;
}

int Piece::GetIdWithoutColor() {
  return object_id_ - 1 + IsWhite();
}

void Piece::Draw(glm::mat4 projection_matrix) {
  if (program_ == nullptr) {
    Logger::Error("program not loaded");
  }
  program_->Use();

  auto textures = ObjectManager::texture.GetTexture(object_id_);
  if (!textures.empty()) {
    textures[0]->Bind();
  }

  glActiveTexture(GL_TEXTURE0 + 3);
  glBindTexture(GL_TEXTURE_2D, shadow_texture);

  program_->Bind(0, "tex");
  program_->Bind(3, "texShadow");

  auto view_projection_shadow = shadow_view_projection * model_view_matrix_;
  auto tra_inv_model_matrix = transpose(inverse(model_view_matrix_));
  program_->Bind(projection_matrix, "view_projection_matrix");
  program_->Bind(view_projection_shadow, "view_projection_shadow");
  program_->Bind(model_view_matrix_, "model_matrix");
  program_->Bind(tra_inv_model_matrix, "tra_inv_model_matrix");

  program_->Bind(light_pos, "lightPos");
  program_->Bind(cam_pos, "camPos");

  glm::vec3 La = glm::vec3(0.5f, 0.5f, 0.5f);
  glm::vec3 ka = glm::vec3(0.5f, 0.5f, 0.5f);
  glm::vec3 Ld = glm::vec3(0.5f, 0.5f, 0.5f);
  glm::vec3 kd = glm::vec3(1, 1, 1);
  program_->Bind(La, "La");
  program_->Bind(ka, "ka");
  program_->Bind(Ld, "Ld");
  program_->Bind(kd, "kd");

  program_->Bind(ObjectManager::texture.Reflectivity(object_id_), "reflectivity");
  program_->Bind(ObjectManager::texture.Shininess(object_id_), "shininess");

  geometry_->Draw();
}

void Piece::DrawShadow(glm::mat4 projection_matrix) {
  program_shadow_->Use();

  auto view_projection_shadow = projection_matrix * model_view_matrix_;
  program_shadow_->Bind(view_projection_shadow, "view_projection_shadow");

  geometry_->Draw();
}

void Piece::Update(float elapsedTimeMs) {
  (void)elapsedTimeMs;

  model_view_matrix_ = translate(glm::mat4(), Position());
  model_view_matrix_ = rotate(model_view_matrix_, y_rotation_, glm::vec3(0, 1, 0));
}

void Piece::MouseClick(glm::vec3 position) {
  // handled by field
  (void)position;
}

void Piece::AddHitOrMove(Field *field, std::vector<MoveBase *> &moves) {
  if (!field) return;
  if (!field->CurrentPiece) {
    moves.push_back(new Move(this, field));
  }
  else if (field->CurrentPiece->IsWhite() != IsWhite()) {
    moves.push_back(new Hit(this, field->CurrentPiece));
  }
}

std::string Piece::GetVertexShader()
{
  return "res/shader/Basic.vs.glsl";
}

std::string Piece::GetFragmentShader()
{
  return "res/shader/Basic.fs.glsl";
}

glm::vec3 Piece::Position3D() {
  return Position();
}
