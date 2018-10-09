#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 * Holds a window and can be used instead of a framebuffer for output.
 */
class Window {
public:
    Window();

    void SetFullScreen();

    int width, height;
    GLFWwindow *handle;

    int getWidth() { return width; }

    int getHeight() { return height; }
};

#endif //WINDOW_H
