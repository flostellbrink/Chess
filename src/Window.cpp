#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Config.h"
#include "Window.h"
#include "Logger.h"

/**
 * Initialize a new window.
 */
Window::Window() : width(), height(), is_full_screen_(Config::full_screen)
{
  if (!glfwInit())
  {
    Logger::Error("Failed to initialize GLFW.");
  }

  glfwWindowHint(GLFW_SAMPLES, 0);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  const auto monitor = glfwGetPrimaryMonitor();
  const auto mode = glfwGetVideoMode(monitor);
  if (is_full_screen_)
  {
    handle = glfwCreateWindow(
      mode->width,
      mode->height,
      "Chess",
      monitor,
      nullptr);
  }
  else
  {
    handle = glfwCreateWindow(
      Config::windowed_width,
      Config::windowed_height,
      "Chess",
      nullptr,
      nullptr);

    glfwSetWindowPos(handle,
                     (mode->width - Config::windowed_width) / 2,
                     (mode->height - Config::windowed_height) / 2);
  }

  if (handle == nullptr)
  {
    Logger::Error("Failed to open GLFW window.");
  }

  UpdateSize();

  glfwMakeContextCurrent(handle);
  glfwSetInputMode(handle, GLFW_STICKY_KEYS, GL_TRUE);
  glfwPollEvents();
  glfwSetCursorPos(handle, width / 2.0, height / 2.0);

  glewExperimental = static_cast<GLboolean>(true);
  if (glewInit() != GLEW_OK)
  {
    Logger::Error("Failed to initialize GLEW.");
  }
}

void Window::Update()
{
  if (is_full_screen_ != Config::full_screen)
  {
    SetFullScreen();
  }
}

/**
 * Set full screen mode
 */
void Window::SetFullScreen()
{
  is_full_screen_ = Config::full_screen;
  const auto monitor = glfwGetPrimaryMonitor();
  const auto mode = glfwGetVideoMode(monitor);
  if (is_full_screen_)
  {
    glfwSetWindowMonitor(
      handle,
      monitor,
      0,
      0,
      mode->width,
      mode->height,
      0);
  }
  else
  {
    glfwSetWindowMonitor(
      handle,
      nullptr,
      (mode->width - Config::windowed_width) / 2,
      (mode->height - Config::windowed_height) / 2,
      Config::windowed_width,
      Config::windowed_height,
      0);
  }

  UpdateSize();
}

void Window::UpdateSize()
{
  glfwGetFramebufferSize(handle, &width, &height);
  Config::viewport_width = width;
  Config::viewport_height = height;
}
