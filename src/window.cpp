#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "config.h"
#include "window.h"
#include "logger.h"

void Window::UpdateSize()
{
    glfwGetFramebufferSize(handle, &width, &height);
    Config::viewportWidth = width;
    Config::viewportHeight = height;
}

/**
 * Initialize a new window.
 */
Window::Window() {
    if (!glfwInit()) {
        Logger::error("Failed to initialize GLFW.");
    }

    glfwWindowHint(GLFW_SAMPLES, 0);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (Config::fullScreen) {
        auto monitor = glfwGetPrimaryMonitor();
        auto video_mode = glfwGetVideoMode(monitor);
        handle = glfwCreateWindow(video_mode->width, video_mode->height, "Chess", monitor, nullptr);
    } else {
        handle = glfwCreateWindow(1280, 720, "Chess", nullptr, nullptr);
    }

    if (handle == nullptr) {
        Logger::error("Failed to open GLFW window.");
    }
    glfwMakeContextCurrent(handle);

    UpdateSize();

    glfwSetInputMode(handle, GLFW_STICKY_KEYS, GL_TRUE);

    glfwPollEvents();
    glfwSetCursorPos(handle, width / 2, height / 2);

    glewExperimental = static_cast<GLboolean>(true);
    if (glewInit() != GLEW_OK) {
        Logger::error("Failed to initialize GLEW.");
    }
}

/**
 * Set full screen mode
 */
void Window::SetFullScreen() {
    if ((glfwGetWindowMonitor(handle) != nullptr) == Config::fullScreen)
        return;

    if (Config::fullScreen) {
        const auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        auto monitor = glfwGetPrimaryMonitor();
        glfwSetWindowMonitor(handle, monitor, 0, 0, mode->width, mode->height, 0);
    } else {
        glfwSetWindowMonitor(handle, nullptr, 0, 0, 1280, 720, 0);
    }

    UpdateSize();
}
