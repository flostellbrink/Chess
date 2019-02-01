#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

class Window
{
public:
  Window();

  int width, height;
  GLFWwindow* handle;

private:
  static void WindowSizeCallback(GLFWwindow* window, int width, int height);
};

#endif //WINDOW_H
