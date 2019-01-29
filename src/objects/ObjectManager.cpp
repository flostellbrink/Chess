#include <GL/glew.h>
#include <vector>
#include <iostream>
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

GeometryManager ObjectManager::geometry;
TextureManager ObjectManager::texture;
AnimationManager ObjectManager::animation;
ObjectManager ObjectManager::instance;
CollisionManager ObjectManager::collision;

const GLuint default_framebuffer = 0;

ObjectManager::ObjectManager() = default;

void ObjectManager::NewGame() {
  objects_.clear();
  post_processors_.clear();
  delete game_board;
  AddObject(&skybox_);
  AddObject(new BasicObject(objects::table, glm::vec3(0, -1, 0), 0, "basic"));
  // Creates all game related objects itself
  game_board = new Board(&camera_);
  clock = new Clock;

  // Post processing
  AddPost(new FullScreenQuad("res/shader/BlurVer.vs.glsl", "res/shader/Blur.fs.glsl"));
  AddPost(new FullScreenQuad("res/shader/BlurHor.vs.glsl", "res/shader/Blur.fs.glsl"));
  AddPost(new FullScreenQuad("res/shader/Overlay.vs.glsl", "res/shader/Overlay.fs.glsl"));
}

void ObjectManager::UpdateFramebuffer(GLuint &framebuffer, GLuint &texture, GLuint &depth, int width, int height) const
{
  // Get rid of old objects
  if (framebuffer)
    glDeleteFramebuffers(1, &framebuffer);
  if (texture) {
    glDeleteTextures(1, &texture);
  }
  if (depth)
    glDeleteRenderbuffers(1, &depth);

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

void ObjectManager::Update(float elapsedTime) {
  if (Config::geo_changed) {
    Config::geo_changed = false;
    geometry.Regenerate();
  }
  if (Config::change_theme) {
    Config::change_theme = false;
    SetTheme((theme_ == themes::wood) ? themes::glass : themes::wood);
  }
  if (Config::undo_turn) {
    Config::undo_turn = false;
    game_board->UndoMove(false);
  }
  if (Config::ai != game_board->GetAi()) {
    game_board->EnableAi(Config::ai);
  }
  if (Config::new_game) {
    std::cerr << "Resetting game" << std::endl;
    Config::new_game = false;
    Board::ResetGame();
    //NewGame();
  }
  if (Config::demo) {
    Config::demo = false;
    game_board->RunDemo();
  }

  if (clock->Timeout()) {
    game_board->SetState(clock->Timeout());
  }

  if (elapsedTime > 200) {
    std::cerr << "ChessWarn: Too much time passed since last update: " << elapsedTime << "ms" << std::endl;
    //This prevents skipping of animations
    elapsedTime = 20;
  }

  for (Drawable *obj : objects_) {
    obj->Update(elapsedTime);
  }
  game_board->Update(elapsedTime);
  camera_.Update();
  animation.Update(elapsedTime);

  clock->Update(elapsedTime, game_board->IsWhitesTurn());
}

/**
 * @brief The DepthSort struct compares objects depth, using a given projection
 */
struct DepthSort {
  explicit DepthSort(glm::mat4 projection) { this->projection = projection; }
  bool operator()(Drawable *obj1, Drawable *obj2) const
  {
    glm::vec4 projected1 = projection * glm::vec4(obj1->Position3D(), 1),
      projected2 = projection * glm::vec4(obj2->Position3D(), 1);
    return projected1.z > projected2.z;
  }

  glm::mat4 projection;
};

void ObjectManager::Draw() {
  // Setup frame buffer
  if (!mirror_frame_buffer_ || !post_frame_buffer_ || !shadow_frame_buffer_ ||
    Config::viewport_width != res_width_ || Config::viewport_height != res_height_) {
    res_width_ = Config::viewport_width;
    res_height_ = Config::viewport_height;
    UpdateFramebuffer(post_frame_buffer_, post_texture_, post_depth_, res_width_, res_height_);
    UpdateFramebuffer(mirror_frame_buffer_, mirror_texture_, mirror_depth_, res_width_, res_height_);
    Drawable::reflection_texture = mirror_texture_;
  }
  if (!shadow_frame_buffer_ || shadow_res_ != Config::shadow_resolution) {
    shadow_res_ = Config::shadow_resolution;
    UpdateFramebuffer(shadow_frame_buffer_, shadow_texture_, shadow_depth_, shadow_res_, shadow_res_);
    Drawable::shadow_texture = shadow_texture_;
  }
  // Get Projection Matrices
  Drawable::cam_pos = camera_.Position();
  glm::mat4 viewProjection = Camera::Projection() * camera_.ViewMat(),
    viewProjectionSkybox = Camera::Projection() * camera_.ViewMatAtCamera(),
    mirror = MirrorMat(glm::vec3(0, 1, 0), 0),
    viewProjectionMirrored = viewProjection * mirror,
    viewProjectionSkyboxMirrored = viewProjectionSkybox * mirror,
    viewProjectionShadow = camera_.ViewProjectionShadow();
  Drawable::shadow_view_projection = viewProjectionShadow;

  // Sort objects by depth
  std::sort(objects_.begin(), objects_.end(), DepthSort(viewProjection));

  // Render Shadow depth map
  glBindFramebuffer(GL_FRAMEBUFFER, shadow_frame_buffer_);
  glViewport(0, 0, shadow_res_, shadow_res_);
  glEnable(GL_DEPTH_TEST);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  for (Drawable *obj : objects_) {
    obj->DrawShadow(viewProjectionShadow);
  }

  // Reset Viewport, assuming all other steps use default resolution
  glViewport(0, 0, res_width_, res_height_);

  // Render Mirrored
  glBindFramebuffer(GL_FRAMEBUFFER, mirror_frame_buffer_);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  skybox_.DrawSkybox(viewProjectionSkyboxMirrored);
  for (Drawable *obj : objects_) {
    obj->DrawReflection(viewProjectionMirrored);
  }

  // Render Background
  glBindFramebuffer(GL_FRAMEBUFFER, post_frame_buffer_);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  skybox_.DrawSkybox(viewProjectionSkybox);

  // Render Main Scene
  for (Drawable *obj : objects_) {
    obj->DrawOpaque(viewProjection);
  }
  for (Drawable *obj : objects_) {
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

  if (click_happened_) {
    click_happened_ = false;
    glm::vec3 mousePos = CheckDepth(last_mouse_, Camera::Projection() * camera_.ViewMat());
    for (Drawable *obj : objects_) {
      obj->MouseClick(mousePos);
    }
  }

  // Do Post and render to screen
  if (post_processors_.empty()) { return; }
  glDisable(GL_DEPTH_TEST);
  bool renderToPost = false;
  glBindFramebuffer(GL_FRAMEBUFFER, mirror_frame_buffer_);
  Drawable::post_texture = post_texture_;

  for (int i = 0; i < static_cast<int>(post_processors_.size()) - 1; ++i) {
    post_processors_[i]->Draw(glm::mat4());
    renderToPost = !renderToPost;
    glBindFramebuffer(GL_FRAMEBUFFER, renderToPost ? post_frame_buffer_ : mirror_frame_buffer_);
    Drawable::post_texture = renderToPost ? mirror_texture_ : post_texture_;
  }

  // Render final pass to screen
  glBindFramebuffer(GL_FRAMEBUFFER, default_framebuffer);
  post_processors_[post_processors_.size() - 1]->Draw(glm::mat4());
  glEnable(GL_DEPTH_TEST);

  /* Blits shadow map when uncommented
  glBindFramebuffer(GL_READ_FRAMEBUFFER, _shadowFrameBuffer);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, defaultFramebuffer);
  glBlitFramebuffer(0, 0, _shadowRes, _shadowRes, 0, 0, Config::viewportWidth, Config::viewportHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
  // */
}

void ObjectManager::AddObject(Drawable *object) {
  objects_.push_back(object);
  object->Init();
}

void ObjectManager::AddPost(Drawable *object) {
  post_processors_.push_back(object);
  object->Init();
}

void ObjectManager::SetTheme(int theme) {
  theme_ = theme;
  geometry.SetTheme(theme);
  texture.SetTheme(theme);
}

void ObjectManager::MouseButton(int button, int action) {
  if (action == GLFW_PRESS) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      click_happened_ = true;
    }
    else {
      camera_.MouseDown();
    }
  }
  else {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      // Ignored
    }
    else {
      camera_.MouseUp();
    }
  }
}

void ObjectManager::MouseMove(double xPos, double yPos) {
  last_mouse_ = glm::vec2(xPos, yPos);
  camera_.MouseMove(last_mouse_);
}

void ObjectManager::MouseWheel(double xOffset, double yOffset) {
  camera_.MouseWheel(static_cast<float>(yOffset));
}

/**
 * @brief ObjectManager::checkDepth Un projects a point from window coordinates to world coordinates
 * @param mousePos The mouse position in window coordinates
 * @param viewProjection The world to window projection
 * @return The mouse position in world coordinates
 */
glm::vec3 ObjectManager::CheckDepth(glm::vec2 mousePos, glm::mat4 viewProjection) const
{
  int width = Config::viewport_width, height = Config::viewport_height;
  if (mousePos.x < 0 || mousePos.x >= width || mousePos.y < 0 || mousePos.y >= height) {
    return glm::vec3();
  }
  mousePos.y = height - mousePos.y;
  float pixel;
  glReadPixels(static_cast<GLint>(mousePos.x),
    static_cast<GLint>(mousePos.y),
    1,
    1,
    GL_DEPTH_COMPONENT,
    GL_FLOAT,
    &pixel);

  glm::vec4 windowCoordinates(mousePos.x / static_cast<float>(width) * 2.f - 1.f,
    mousePos.y / static_cast<float>(height) * 2.f - 1.f,
    pixel * 2.f - 1.f,
    1);
  glm::vec4 result = inverse(viewProjection) * windowCoordinates;
  if (result.w == 0) {
    return glm::vec3();
  }
  result /= result.w;
  return glm::vec3(result);
}

glm::mat4 ObjectManager::MirrorMat(glm::vec3 normal, float distance) const
{
  return glm::mat4(glm::vec4(1 - 2 * normal.x, -2 * normal.x * normal.y, -2 * normal.x * normal.z, 0),
    glm::vec4(-2 * normal.x * normal.y, 1 - 2 * normal.y, -2 * normal.y * normal.z, 0),
    glm::vec4(-2 * normal.x * normal.z, -2 * normal.y * normal.z, 1 - 2 * normal.z, 0),
    glm::vec4(-2 * normal.x * distance, -2 * normal.y * distance, -2 * normal.z * distance, 1));
}
