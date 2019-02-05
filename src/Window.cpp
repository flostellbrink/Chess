#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Config.h"
#include "Window.h"
#include "Logger.h"

void ErrorCallback(int error, const char* description)
{
  std::stringstream message;
  message << "GLFW (" << error << "): " << description;
  Logger::Warn(message.str());
}

/**
 * Initialize a new window.
 */
Window::Window() : width(), height()
{
  if (!glfwInit())
  {
    Logger::Error("Failed to initialize GLFW.");
  }

  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  handle = glfwCreateWindow(
    Config::viewport_width,
    Config::viewport_height,
    "Chess",
    nullptr,
    nullptr);

  if (handle == nullptr)
  {
    Logger::Error("Failed to open GLFW window.");
  }

  glfwSetWindowSizeCallback(handle, WindowSizeCallback);
  glfwMakeContextCurrent(handle);

  glewExperimental = static_cast<GLboolean>(true);
  if (glewInit() != GLEW_OK)
  {
    Logger::Error("Failed to initialize GLEW.");
  }
}

void Window::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
  std::stringstream message;
  message << "Resizing to " << width << ", " << height;
  Logger::Info(message.str());

  Config::viewport_width = width;
  Config::viewport_height = height;
}
