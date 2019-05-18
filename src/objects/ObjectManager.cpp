#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <GLFW/glfw3.h>
#include "ObjectManager.h"
#include "src/Objects.h"
#include "Drawable.h"
#include "Board.h"
#include "Skybox.h"
#include "BasicObject.h"
#include "src/Config.h"
#include "FullScreenQuad.h"
#include "Clock.h"
#include "src/Logger.h"

GeometryManager ObjectManager::geometry;
TextureManager ObjectManager::texture;
AnimationManager ObjectManager::animation;
ObjectManager ObjectManager::instance;
CollisionManager ObjectManager::collision;

const GLuint default_framebuffer = 0;

ObjectManager::ObjectManager() = default;

void ObjectManager::NewGame()
{
  objects_.clear();
  post_processors_.clear();
  delete game_board;
  skybox_.Init();
  AddObject(new BasicObject(objects::table, glm::vec3(0, -1, 0), 0, "Basic"));
  // Creates all game related objects itself
  game_board = new Board(&camera_);
  clock = new Clock;

  // Post processing
  AddPost(new FullScreenQuad("res/shader/BlurVer.vs.glsl", "res/shader/Blur.fs.glsl"));
  AddPost(new FullScreenQuad("res/shader/BlurHor.vs.glsl", "res/shader/Blur.fs.glsl"));
  AddPost(new FullScreenQuad("res/shader/Overlay.vs.glsl", "res/shader/Overlay.fs.glsl"));
}

void ObjectManager::UpdateFramebuffer(GLuint& framebuffer,
                                      GLuint& texture,
                                      GLuint& depth,
                                      const int width,
                                      const int height)
{
  // Get rid of old objects
  if (framebuffer)
  {
    glDeleteFramebuffers(1, &framebuffer);
  }

  if (texture)
  {
    glDeleteTextures(1, &texture);
  }

  if (depth)
  {
    glDeleteRenderbuffers(1, &depth);
  }

  // Generate Frame Buffer
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  // Generate Texture
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // Generate Depth Buffer
  glGenRenderbuffers(1, &depth);
  glBindRenderbuffer(GL_RENDERBUFFER, depth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

  // Setup Frame Buffer
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);

  assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void ObjectManager::Update(float elapsedTime)
{
  if (Config::geo_changed)
  {
    Config::geo_changed = false;
    geometry.Regenerate();
  }
  if (Config::change_theme)
  {
    Config::change_theme = false;
    SetTheme(theme_ == themes::wood ? themes::glass : themes::wood);
  }
  if (Config::undo_turn)
  {
    Config::undo_turn = false;
    game_board->UndoMove(false);
  }
  if (Config::ai != game_board->GetAi())
  {
    game_board->EnableAi(Config::ai);
  }
  if (Config::new_game)
  {
    Logger::Info("Resetting game");
    Config::new_game = false;
    Board::ResetGame();
  }
  if (Config::demo)
  {
    Logger::Info("Resetting game and running demo");
    Config::demo = false;
    Board::ResetGame();
    game_board->RunDemo();
  }

  if (clock->Timeout())
  {
    game_board->SetState(clock->Timeout());
  }

  if (elapsedTime > 200)
  {
    std::stringstream message;
    message << "Too much time passed since last update: " << elapsedTime << "ms";
    Logger::Warn(message.str());

    //This prevents skipping of animations
    elapsedTime = 20;
  }

  for (auto obj : objects_)
  {
    obj->Update(elapsedTime);
  }
  game_board->Update(elapsedTime);
  animation.Update(elapsedTime);

  clock->Update(elapsedTime, game_board->IsWhitesTurn());
}

/**
 * @brief The DepthSort struct compares objects depth, using a given projection
 */
struct DepthSort
{
  explicit DepthSort(const glm::mat4 projection) { this->projection = projection; }

  bool operator()(Drawable* obj1, Drawable* obj2) const
  {
    const auto projected1 = projection * glm::vec4(obj1->Position3D(), 1);
    const auto projected2 = projection * glm::vec4(obj2->Position3D(), 1);
    return projected1.z < projected2.z;
  }

  glm::mat4 projection;
};

void ObjectManager::Draw()
{
  // Setup frame buffer
  if (!mirror_frame_buffer_ || !post_frame_buffer_ || !post_frame_buffer_2_ || !shadow_frame_buffer_ ||
    Config::viewport_width != res_width_ || Config::viewport_height != res_height_)
  {
    if (Config::viewport_width <= 1 || Config::viewport_height <= 1)
    {
      Logger::Warn("Viewport empty");
      return;
    }

    res_width_ = Config::viewport_width;
    res_height_ = Config::viewport_height;
    UpdateFramebuffer(post_frame_buffer_, post_texture_, post_depth_, res_width_, res_height_);
    UpdateFramebuffer(post_frame_buffer_2_, post_texture_2_, post_depth_2_, res_width_, res_height_);

    // Render mirror at quarter resolution
    UpdateFramebuffer(mirror_frame_buffer_, mirror_texture_, mirror_depth_, res_width_ / 2, res_height_ / 2);
    Drawable::reflection_texture = mirror_texture_;

    // Render shadow map at quarter resolution (square based on width)
    UpdateFramebuffer(shadow_frame_buffer_, shadow_texture_, shadow_depth_, res_width_ / 2, res_width_ / 2);
    Drawable::shadow_texture = shadow_texture_;
  }
  
  // Get Projection Matrices
  Drawable::cam_pos = camera_.Position();
  const auto viewProjection = Camera::Projection() * camera_.ViewMat();
  const auto viewProjectionSkybox = Camera::Projection() * camera_.ViewMatAtCamera();
  const auto mirror = MirrorMat(glm::vec3(0, 1, 0), 0);
  const auto viewProjectionMirrored = viewProjection * mirror;
  const auto viewProjectionSkyboxMirrored = viewProjectionSkybox * mirror;
  const auto viewProjectionShadow = camera_.ViewProjectionShadow();
  Drawable::shadow_view_projection = viewProjectionShadow;

  if (mouse_down_happened_)
  {
    mouse_down_happened_ = false;
    mouse_down_object_ = GetClickedObject(last_mouse_, Camera::Projection() * camera_.ViewMat());
  }

  if (mouse_up_happened_)
  {
    mouse_up_happened_ = false;
    auto mouse_up_object = GetClickedObject(last_mouse_, Camera::Projection() * camera_.ViewMat());
    if (mouse_down_object_ != nullptr && mouse_down_object_ == mouse_up_object)
    {
      mouse_up_object->MouseClick();
    }
    mouse_down_object_ = nullptr;
  }

  // Sort objects by depth for fast rendering
  std::sort(objects_.begin(), objects_.end(), DepthSort(viewProjection));

  // Render Shadow depth map
  glBindFramebuffer(GL_FRAMEBUFFER, shadow_frame_buffer_);
  glViewport(0, 0, res_width_ / 2, res_width_ / 2);
  glEnable(GL_DEPTH_TEST);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  for (auto obj : objects_)
  {
    obj->DrawShadow(viewProjectionShadow);
  }

  // Sort objects by depth for fast rendering
  std::sort(objects_.begin(), objects_.end(), DepthSort(viewProjectionMirrored));

  // Render Mirrored
  glBindFramebuffer(GL_FRAMEBUFFER, mirror_frame_buffer_);
  glViewport(0, 0, res_width_ / 2, res_height_ / 2);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  skybox_.DrawSkybox(viewProjectionSkyboxMirrored);
  for (auto obj : objects_)
  {
    obj->DrawReflection(viewProjectionMirrored);
  }

  // Render Background
  glBindFramebuffer(GL_FRAMEBUFFER, post_frame_buffer_);
  glViewport(0, 0, res_width_, res_height_);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  skybox_.DrawSkybox(viewProjectionSkybox);

  // Sort objects by depth for fast rendering
  std::sort(objects_.begin(), objects_.end(), DepthSort(viewProjection));

  // Render Main Scene
  for (auto obj : objects_)
  {
    obj->DrawOpaque(viewProjection);
  }

  // Render Translucent Main Scene (reverse order for correctness)
  std::reverse(objects_.begin(), objects_.end());
  for (auto obj : objects_)
  {
    // Draw depth
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    obj->DrawTranslucent(viewProjection);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    // Draw surface color
    glDepthFunc(GL_EQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    obj->DrawTranslucent(viewProjection);
    glDisable(GL_BLEND);
    glDepthFunc(GL_LESS);
  }

  // Do Post and render to screen
  if (post_processors_.empty()) { return; }
  glDisable(GL_DEPTH_TEST);
  auto renderToPost = false;
  glBindFramebuffer(GL_FRAMEBUFFER, post_frame_buffer_2_);
  Drawable::post_texture = post_texture_;

  for (auto i = 0; i < static_cast<int>(post_processors_.size()) - 1; ++i)
  {
    post_processors_[i]->Draw(glm::mat4(1.0f));
    renderToPost = !renderToPost;
    glBindFramebuffer(GL_FRAMEBUFFER, renderToPost ? post_frame_buffer_ : post_frame_buffer_2_);
    Drawable::post_texture = renderToPost ? post_texture_2_ : post_texture_;
  }

  // Render final pass to screen
  glBindFramebuffer(GL_FRAMEBUFFER, default_framebuffer);
  glViewport(0, 0, Config::viewport_width, Config::viewport_height);
  post_processors_[post_processors_.size() - 1]->Draw(glm::mat4(1.0f));
  glEnable(GL_DEPTH_TEST);

  /* Blits shadow map when uncommented
  glBindFramebuffer(GL_READ_FRAMEBUFFER, shadow_frame_buffer_);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, default_framebuffer);
  glBlitFramebuffer(0, 0, res_width_ / 2, res_width_ / 2, 0, 0, Config::viewport_width, Config::viewport_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
  // */
}

void ObjectManager::AddObject(Drawable* object)
{
  objects_.push_back(object);
  object->Init();
}

void ObjectManager::AddPost(Drawable* object)
{
  post_processors_.push_back(object);
  object->Init();
}

void ObjectManager::SetTheme(const int theme)
{
  theme_ = theme;
  geometry.SetTheme(theme);
  texture.SetTheme(theme);
}

void ObjectManager::MouseButton(const int button, const int action)
{
  if (action == GLFW_PRESS)
  {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
      camera_.MouseDown();
      mouse_down_happened_ = true;
    }
  }
  else
  {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
      camera_.MouseUp();
      mouse_up_happened_ = true;
    }
  }
}

void ObjectManager::MouseMove(const double xPos, const double yPos)
{
  last_mouse_ = glm::vec2(xPos, yPos);
  camera_.MouseMove(last_mouse_);
}

void ObjectManager::MouseWheel(double xOffset, const double yOffset)
{
  (void)xOffset;

  camera_.MouseWheel(static_cast<float>(yOffset));
}

/**
 * Finds an object id by rendering all object ids into a texture and reading the pixel at mousePos.
 */
Drawable* ObjectManager::GetClickedObject(glm::vec2 mousePos, const glm::mat4 viewProjection) const
{
  const auto width = Config::viewport_width;
  const auto height = Config::viewport_height;
  if (mousePos.x < 0 || mousePos.x >= width || mousePos.y < 0 || mousePos.y >= height)
  {
    return nullptr;
  }
  mousePos.y = height - mousePos.y;

  glBindFramebuffer(GL_FRAMEBUFFER, post_frame_buffer_);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  assert(objects_.size() <= 256);
  auto index = 0;
  for (auto obj : objects_)
  {
    obj->DrawId(viewProjection, index++);
  }

  unsigned char pixel[4];
  glReadPixels(static_cast<GLint>(mousePos.x),
    static_cast<GLint>(mousePos.y),
    1,
    1,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    &pixel);

  int id = std::round(pixel[0]);
  if(id < 0 || id >= objects_.size())
  {
    return nullptr;
  }

  return objects_[id];
}

glm::mat4 ObjectManager::MirrorMat(const glm::vec3 normal, const float distance)
{
  return glm::mat4(glm::vec4(1 - 2 * normal.x, -2 * normal.x * normal.y, -2 * normal.x * normal.z, 0),
                   glm::vec4(-2 * normal.x * normal.y, 1 - 2 * normal.y, -2 * normal.y * normal.z, 0),
                   glm::vec4(-2 * normal.x * normal.z, -2 * normal.y * normal.z, 1 - 2 * normal.z, 0),
                   glm::vec4(-2 * normal.x * distance, -2 * normal.y * distance, -2 * normal.z * distance, 1));
}
