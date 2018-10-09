#include <cstdio>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <src/objects/objectManager.hpp>
#include "config.h"
#include "logger.h"
#include "window.h"

std::shared_ptr<Window> window;

/**
 * OpenGL callback for warnings and errors.
 */
void messageCallback(GLenum source,
                     GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length,
                     const GLchar *message,
                     const void *userParam) {
    // OpenGL debug callback - great location for a breakpoint ;)
    Logger::warn(message);
}

/**
 * Toggle a boolean value based on a key code.
 * @param value Property to toggle.
 * @param name Name of property to output to console.
 * @param key Current key code.
 * @param toggle_key Key code for toggling property.
 */
void toggle(bool &value, const std::string &name, int key, int toggle_key) {
    if (key == toggle_key) {
        value ^= 1;
        std::stringstream message;
        message << "Toggled " << name << " to " << (value ? "on" : "off") << ".";
        Logger::info(message.str());
    }
}

/**
 * Sets a boolean value based on a key code.
 * @param value Property to toggle.
 * @param name Name of property to output to console.
 * @param key Current key code.
 * @param toggle_key Key code for toggling property.
 */
void signal(bool &value, const std::string &name, int key, int toggle_key) {
    if (key == toggle_key) {
        value = true;
        std::stringstream message;
        message << "Signaled " << name << ".";
        Logger::info(message.str());
    }
}

/**
 * Modify a numeric value based on two key codes.
 * @tparam Numeric Numeric type to update.
 * @param value Value to update.
 * @param name Name of property to output to console.
 * @param key Current key code.
 * @param increase_key Key code for increasing the value.
 * @param decrease_key Key code for decreasing the value.
 * @param min Minimum value for decreasing the property.
 * @param max Maximum value for increasing the property.
 * @param step Step size for increments and decrements.
 */
template <typename Numeric>
void modify(Numeric& value, const std::string &name, int key, int increase_key, int decrease_key,
            Numeric min, Numeric max, Numeric step)
{
    std::stringstream message;
    if (key == increase_key && value < max) {
        value += step;
        message << "Increased " << name << " to " << value << ".";
        Logger::info(message.str());
    } else if (key == decrease_key && value > min) {
        value -= step;
        message << "Decreased " << name << " to " << value << ".";
        Logger::info(message.str());
    }
}

/**
 * Callback on key state change. Applies changes to config.
 * @param handle Window on which key event was registered.
 * @param key Key with state change.
 * @param scancode Scancode of change.
 * @param action Registered key action.
 * @param mods Key modifiers.
 */
void keyCallback(GLFWwindow *handle, int key, int scancode, int action, int mods) {
    if(action != GLFW_PRESS) {
        return;
    }

    signal(Config::undoTurn, "undo turn", key, GLFW_KEY_R);
    signal(Config::changeTheme, "change theme", key, GLFW_KEY_T);
    signal(Config::newGame, "new game", key, GLFW_KEY_DELETE);
    signal(Config::demo, "demo", key, GLFW_KEY_DELETE);

    if (key == GLFW_KEY_F) {
        toggle(Config::fullScreen, "full screen", key, GLFW_KEY_F);
        window->SetFullScreen();
    }

    if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) {
        glfwSetWindowShouldClose(handle, 1);
    }
}

/**
 * Entry point for engine.
 * @param argc Number of arguments.
 * @param argv Arguments pointer.
 * @return Exit code.
 */
int main(int argc, char *argv[]) {
    window = std::make_shared<Window>();
    try {
        // Setup window and debug callback
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback((GLDEBUGPROC) messageCallback, nullptr);

        // Setup input callback
        glfwSetKeyCallback(window->handle, keyCallback);

        // Create game and renderer
        ObjectManager::Instance.NewGame();

        double lastFrameTime = glfwGetTime();
        do {
            auto currentFrame = glfwGetTime();
            auto deltaTime = static_cast<float>(currentFrame - lastFrameTime);
            lastFrameTime = currentFrame;

            // Update and render game
            ObjectManager::Instance.Update(deltaTime, glm::perspective(90.0f, 16.f/9.f, 0.1f, 100.0f));
            ObjectManager::Instance.Draw(glm::perspective(90.0f, 16.f/9.f, 0.1f, 100.0f));

            glfwSwapBuffers(window->handle);
            glfwPollEvents();
        } while (glfwWindowShouldClose(window->handle) == GLFW_FALSE);
        glfwTerminate();
        return 0;
    } catch (std::exception &exception) {
        Logger::warn("Fatal: " + std::string(exception.what()));
        return 1;
    }
}
