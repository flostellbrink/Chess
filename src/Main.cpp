#include <cstdio>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <src/objects/ObjectManager.h>
#include "Config.h"
#include "Logger.h"
#include "Window.h"

/**
 * OpenGL callback for warnings and errors.
 */
#if !(defined(_WIN32) || defined(WIN32))
#define __stdcall
#endif
void __stdcall message_callback(GLenum source,
                                GLenum type,
                                GLuint id,
                                const GLenum severity,
                                GLsizei length,
                                const GLchar *message,
                                const void *userParam) {
  // OpenGL debug callback - great location for a breakpoint ;)
  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    Logger::Error(message);
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    // Do not log info messages
    // Logger::info(message);
    break;
  default:
    Logger::Warn(message);
    break;
  }
}

/**
 * Toggle a boolean value based on a key code.
 * @param value Property to toggle.
 * @param name Name of property to output to console.
 * @param key Current key code.
 * @param toggleKey Key code for toggling property.
 */
void toggle(bool &value, const std::string &name, const int key, const int toggleKey) {
  if (key == toggleKey) {
    value ^= 1;
    std::stringstream message;
    message << "Toggled " << name << " to " << (value ? "on" : "off") << ".";
    Logger::Info(message.str());
  }
}

/**
 * Sets a boolean value based on a key code.
 * @param value Property to toggle.
 * @param name Name of property to output to console.
 * @param key Current key code.
 * @param signalKey Key code for signaling property.
 */
void signal(bool &value, const std::string &name, const int key, const int signalKey) {
  if (key == signalKey) {
    value = true;
    std::stringstream message;
    message << "Signaled " << name << ".";
    Logger::Info(message.str());
  }
}

/**
 * Modify a numeric value based on two key codes.
 * @tparam Numeric Numeric type to update.
 * @param value Value to update.
 * @param name Name of property to output to console.
 * @param key Current key code.
 * @param increaseKey Key code for increasing the value.
 * @param decreaseKey Key code for decreasing the value.
 * @param min Minimum value for decreasing the property.
 * @param max Maximum value for increasing the property.
 * @param step Step size for increments and decrements.
 */
template <typename Numeric>
void modify(Numeric& value,
            const std::string &name,
            const int key,
            const int increaseKey,
            const int decreaseKey,
            Numeric min,
            Numeric max,
            Numeric step)
{
  std::stringstream message;
  if (key == increaseKey && value < max) {
    value += step;
    message << "Increased " << name << " to " << value << ".";
    Logger::Info(message.str());
  }
  else if (key == decreaseKey && value > min) {
    value -= step;
    message << "Decreased " << name << " to " << value << ".";
    Logger::Info(message.str());
  }
}

/**
 * Callback on key state change. Applies changes to config.
 * @param handle Window on which key event was registered.
 * @param key Key with state change.
 * @param scanCode Scan code of change.
 * @param action Registered key action.
 * @param mods Key modifiers.
 */
void key_callback(GLFWwindow *handle, const int key, int scanCode, const int action, int mods) {
  if (action != GLFW_PRESS) {
    return;
  }

  signal(Config::demo, "demo", key, GLFW_KEY_D);
  toggle(Config::ai, "artificial intelligence", key, GLFW_KEY_A);
  signal(Config::undo_turn, "undo turn", key, GLFW_KEY_Z);
  signal(Config::new_game, "new game", key, GLFW_KEY_N);
  signal(Config::change_theme, "change theme", key, GLFW_KEY_T);
  modify(Config::geo_resolution1, "horizontal resolution", key, GLFW_KEY_U, GLFW_KEY_J, 3, 100, 1);
  modify(Config::geo_resolution2, "vertical resolution", key, GLFW_KEY_I, GLFW_KEY_K, 3, 100, 1);
  signal(Config::geo_changed, "geometry changed", key, GLFW_KEY_U);
  signal(Config::geo_changed, "geometry changed", key, GLFW_KEY_J);
  signal(Config::geo_changed, "geometry changed", key, GLFW_KEY_I);
  signal(Config::geo_changed, "geometry changed", key, GLFW_KEY_K);

  if (key == GLFW_KEY_F) {
    toggle(Config::full_screen, "full screen", key, GLFW_KEY_F);
  }

  if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) {
    glfwSetWindowShouldClose(handle, 1);
  }
}

/**
 * Callback on cursor movement.
 */
static void cursor_pos_callback(GLFWwindow* window, const double xPos, const double yPos)
{
  ObjectManager::instance.MouseMove(xPos, yPos);
}

/**
 * Callback on mouse button clicks.
 */
void mouse_button_callback(GLFWwindow* window, const int button, const int action, int mods)
{
  ObjectManager::instance.MouseButton(button, action);
}

/**
 * Callback on scroll wheel movement.
 */
void scroll_callback(GLFWwindow* window, const double xOffset, const double yOffset)
{
  ObjectManager::instance.MouseWheel(xOffset, yOffset);
}

/**
 * Entry point for engine.
 * @param argc Number of arguments.
 * @param argv Arguments pointer.
 * @return Exit code.
 */
int main(int argc, char *argv[]) {
  try {
    Window window;

    // Setup window and debug callback
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(static_cast<GLDEBUGPROC>(message_callback), nullptr);

    // Setup input callback
    glfwSetKeyCallback(window.handle, key_callback);
    glfwSetCursorPosCallback(window.handle, cursor_pos_callback);
    glfwSetMouseButtonCallback(window.handle, mouse_button_callback);
    glfwSetScrollCallback(window.handle, scroll_callback);

    // Create game and renderer
    ObjectManager::instance.NewGame();

    auto lastFrameTime = glfwGetTime();
    do {
      const auto currentFrame = glfwGetTime();
      const auto deltaTime = static_cast<float>(currentFrame - lastFrameTime);
      lastFrameTime = currentFrame;

      // Update and render game
      window.Update();
      ObjectManager::instance.Update(deltaTime * 1000);
      ObjectManager::instance.Draw();

      glfwSwapBuffers(window.handle);
      glfwPollEvents();
    } while (glfwWindowShouldClose(window.handle) == GLFW_FALSE);

    glfwTerminate();
    return 0;
  }
  catch (std::exception &exception) {
    Logger::Warn("Fatal: " + std::string(exception.what()));
    return 1;
  }
}
