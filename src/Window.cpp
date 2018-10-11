#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Config.h"
#include "Window.h"
#include "Logger.h"

void Window::UpdateSize()
{
  glfwGetFramebufferSize(handle, &width, &height);
  Config::viewport_width = width;
  Config::viewport_height = height;
  is_full_screen_ = glfwGetWindowMonitor(handle) != nullptr;
}

/**
 * Initialize a new window.
 */
Window::Window() : width(), height() {
  if (!glfwInit()) {
    Logger::Error("Failed to initialize GLFW.");
  }

  glfwWindowHint(GLFW_SAMPLES, 0);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  handle = glfwCreateWindow(1280, 720, "Chess", nullptr, nullptr);

  if (handle == nullptr) {
    Logger::Error("Failed to open GLFW window.");
  }
  SetFullScreen();

  glfwMakeContextCurrent(handle);
  glfwSetInputMode(handle, GLFW_STICKY_KEYS, GL_TRUE);
  glfwPollEvents();
  glfwSetCursorPos(handle, width / 2.0, height / 2.0);

  glewExperimental = static_cast<GLboolean>(true);
  if (glewInit() != GLEW_OK) {
    Logger::Error("Failed to initialize GLEW.");
  }
}

/**
 * Set full screen mode
 */
void Window::SetFullScreen() {
  const auto monitor = glfwGetPrimaryMonitor();
  const auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  if (Config::full_screen) {
    glfwSetWindowMonitor(handle, monitor, 0, 0, mode->width, mode->height, 0);
  }
  else {
    glfwSetWindowMonitor(handle, nullptr, 0, 0, 1280, 720, 0);
    glfwSetWindowPos(handle, (mode->width - 1280) / 2, (mode->height - 720) / 2);
  }

  UpdateSize();
}

void Window::Update()
{
  if(is_full_screen_ != Config::full_screen)
  {
    SetFullScreen();
  }
}
