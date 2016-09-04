// has update and draw methods
// base class for all drawable objects
// has a position & rotation

#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <string>

#define GLM_FORCE_RADIANS
#include <glm/mat4x4.hpp>

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glew.h>

#include "texture/texManager.hpp"
#include "geometry/geoManager.hpp"

using glm::mat4;

/**
 * @brief The Drawable class manages drawable objects
 *
 * Hint: This is an abstract class
 */
class Drawable{

public:

    Drawable(int objectId = 0);

    /**
     * @brief init initializes the object
     *
     * The shaders are loaded and the object gets created
     */
    virtual void init();

    /**
     * @brief draw Draws the objects
     * @param projection_matrix The current projection matrix
     *
     * The method draws the objects on the current gl context.
     *
     * Hint: This method is currently pure virtual. You can
     * implement it in the drawable.cpp to avoid redundant
     * code.
     */
    virtual void draw(mat4 projection_matrix) = 0;
    virtual void drawOpaque(mat4 projection_matrix);
    virtual void drawShadow(mat4 projection_matrix) = 0;
    virtual void drawReflection(mat4 projection_matrix);
    virtual void drawTranslucent(mat4 projection_matrix);

    virtual void recreateGeoemtry() = 0;

    virtual vec3 Position3D() = 0;

    /**
     * @brief update Updates the object's position, rotation etc.
     * @param elapsedTimeMs The elapsed time since the last update in ms
     * @param modelViewMatrix the mode view matrix of the parent object
     *
     * Hint: use this method to update the _modelViewMatrix
     */
    virtual void update(float elapsedTimeMs) = 0;

    virtual void MouseClick(vec3 position) = 0;

    static vec3 _lightPos;
    static vec3 _camPos;

    static GLuint ReflectionTexture, PostTexture, ShadowTexture;
    static mat4 ShadowViewProjection;
    static int OverlayState;
    static float OverlayOpacity;

protected:

    GLuint _program = 0, _programShadow = 0;            /**< The opengl program handling the shaders */
    GLuint _vertexArrayObject = 0;  /**< The vertex array object containing the vertices */
    glm::mat4 _modelViewMatrix; /**< The model view matrix to get the object into model view space */
    int _objectId;

    /**
     * @brief initShader Initializes the shader.
     *
     * The function loads a vertex and a fragment shader into
     * the _program.
     */
    virtual void initShader();

    /**
     * @brief loadShaderFile Loads a shader from a given path
     * @param path The path to the shader
     * @return The content of the shader file
     *
     * Hint: You can use the Qt Resource System for the path
     * (e.g. ":/shader/shader.fs.glsl")
     */
    virtual std::string loadShaderFile(std::string path);

    /**
     * @brief getVertexShader Getter for the vertex shader
     * @return the vertex shader as string
     *
     * This method must return the shader as string,
     * it must not return the path to the shader.
     *
     * Hint: use loadShaderFile(std::string path)
     */
    virtual std::string getVertexShader() = 0;

    /**
     * @brief getFragmentShader Getter for the fragment shader
     * @return the fragment shader as string
     *
     * This method must return the shader as string,
     * it must not return the path to the shader.
     *
     * Hint: use loadShaderFile(std::string path)
     */
    virtual std::string getFragmentShader() = 0;
};


#endif // DRAWABLE_H
