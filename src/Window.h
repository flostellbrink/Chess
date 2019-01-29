#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

class Window
{
public:
  Window();

  void Update();

  int width, height;
  GLFWwindow* handle;

private:
  void SetFullScreen();
  void UpdateSize();

  bool is_full_screen_{};
};

#endif //WINDOW_H
