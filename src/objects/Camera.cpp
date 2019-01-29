#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <src/Config.h>

#include "Camera.h"
#include "ObjectManager.h"
#include "src/animation/FadeAnimation.h"
#include "src/animation/DelayAnimation.h"

const float cam_speed = 0.01f, cam_zoom = 10;

Camera::Camera()
  : zoom_factor_(3.0f),
    camera_rotation_(0, -0.75f),
    smooth_camera_rotation_(0, -0.75f),
    mouse_moving_(false),
    white_side_(true)
{
}

glm::vec3 Camera::Position() const
{
  const auto y = smooth_camera_rotation_.y + auto_rotation_.y;
  const auto x = smooth_camera_rotation_.x + auto_rotation_.x;
  return cam_zoom * glm::vec3(sin(y) * cos(x), cos(y), sin(x) * sin(y));
}

glm::vec3 Camera::Up() const
{
  const auto y = smooth_camera_rotation_.y + auto_rotation_.y;
  const auto x = smooth_camera_rotation_.x + auto_rotation_.x;
  return cam_zoom * glm::vec3(sin(y + 0.1f) * cos(x), cos(y + 0.1f), sin(x) * sin(y + 0.1f)) - Position();
}

glm::mat4 Camera::ViewMat() const
{
  return lookAt(zoom_factor_ * Position(), glm::vec3(), Up());
}

glm::mat4 Camera::ViewMatAtCamera() const
{
  return lookAt(glm::vec3(), -Position(), Up());
}

glm::mat4 Camera::ViewProjectionShadow() const
{
  const auto lightPos = Drawable::light_pos;
  const auto shadowProjection = glm::ortho<float>(-30, 30, -30, 30, 10, 100);
  const auto shadowView = lookAt(lightPos, glm::vec3(), glm::vec3(0, 1, 0));

  return shadowProjection * shadowView;
}

glm::mat4 Camera::Projection()
{
  return glm::perspective(1.0f, Config::viewport_width / static_cast<float>(Config::viewport_height), 0.1f, 100.0f);
}

void Camera::Update()
{
  smooth_camera_rotation_ += (camera_rotation_ - smooth_camera_rotation_) * 0.1f;
}

void Camera::SetBoardSide(const bool whiteSide)
{
  ObjectManager::animation.PlayLast(new DelayAnimation<float>(500));

  if (whiteSide == white_side_)
  {
    return;
  }
  white_side_ = whiteSide;

  const auto resetPos = glm::vec2(0, -0.75f);
  auto_rotation_ += smooth_camera_rotation_ - resetPos;
  smooth_camera_rotation_ = camera_rotation_ = resetPos;

  // ensure the camera will always rotate along the smaller angle
  if (whiteSide && auto_rotation_.x > glm::pi<float>())
    auto_rotation_.x -= 2.f * glm::pi<float>();
  if (!whiteSide && auto_rotation_.x < 0)
    auto_rotation_.x += 2.f * glm::pi<float>();
  ObjectManager::animation.PlayLast(
    new FadeAnimation<glm::vec2>(1500, auto_rotation_, auto_rotation_, glm::vec2(whiteSide ? 0 : glm::pi<float>(), 0)));
}

void Camera::MouseDown()
{
  mouse_moving_ = true;
}

void Camera::MouseUp()
{
  mouse_moving_ = false;
}

void Camera::MouseMove(const glm::vec2 mousePos)
{
  if (mouse_moving_)
  {
    camera_rotation_ += (mousePos - old_mouse) * cam_speed;
  }
  old_mouse = mousePos;

  static auto quarterRot = glm::pi<float>() / 2.f;
  if (camera_rotation_.x > quarterRot)
    camera_rotation_.x = quarterRot;
  if (camera_rotation_.x < -quarterRot)
    camera_rotation_.x = -quarterRot;
  if (camera_rotation_.y > 0)
    camera_rotation_.y = 0;
  if (camera_rotation_.y < -quarterRot)
    camera_rotation_.y = -quarterRot;
}

void Camera::MouseWheel(const float wheelDelta)
{
  zoom_factor_ *= std::pow(1.1f, wheelDelta);
  zoom_factor_ = glm::clamp(zoom_factor_, 1.0f, 5.0f);
}
