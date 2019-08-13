#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <GL/glew.h>
#include <map>
#include <src/Logger.h>
#include "Shader.h"

Shader* Shader::GetShaderCached(const std::string& vertexFilePath, const std::string& fragmentFilePath)
{
  const auto key = vertexFilePath + fragmentFilePath;

  const auto iterator = shaders_.find(key);
  if (iterator != shaders_.end())
  {
    return iterator->second;
  }

  const auto shader = new Shader(vertexFilePath, fragmentFilePath);
  shaders_[key] = shader;
  return shader;
}

/**
 * Use program for rendering.
 */
void Shader::Use() const
{
  glUseProgram(handle);
}

/**
 * Get handle to uniform variable by name.
 * @param uniformName Name of variable to get handle for.
 * @return Handle of uniform variable.
 */
GLint Shader::GetUniform(const std::string& uniformName) const
{
  const auto result = glGetUniformLocation(handle, uniformName.data());
  if (result < 0)
  {
    Logger::Error("Could not find uniform: " + uniformName);
  }
  return result;
}

bool Shader::HasUniform(const std::string& uniformName) const
{
  return glGetUniformLocation(handle, uniformName.data()) > 0;
}

/**
 * Bind scalar value to shader program.
 * @param scalar Value to bind to shader.
 * @param uniformName Name of variable to bind value to.
 */
void Shader::Bind(const int scalar, const std::string& uniformName) const
{
  glUniform1i(GetUniform(uniformName), scalar);
}

/**
 * Bind scalar value to shader program.
 * @param scalar Value to bind to shader.
 * @param uniformName Name of variable to bind value to.
 */
void Shader::Bind(const float scalar, const std::string& uniformName) const
{
  glUniform1f(GetUniform(uniformName), scalar);
}

/**
 * Bind vector value to shader program.
 * @param vector Value to bind to shader.
 * @param uniformName Name of variable to bind vector to.
 */
void Shader::Bind(glm::vec2& vector, const std::string& uniformName) const
{
  glUniform2f(GetUniform(uniformName), vector.x, vector.y);
}

/**
 * Bind vector value to shader program.
 * @param vector Value to bind to shader.
 * @param uniformName Name of variable to bind vector to.
 */
void Shader::Bind(glm::vec3& vector, const std::string& uniformName) const
{
  glUniform3f(GetUniform(uniformName), vector.x, vector.y, vector.z);
}

/**
 * Bind vector value to shader program.
 * @param vector Value to bind to shader.
 * @param uniformName Name of variable to bind vector to.
 */
void Shader::Bind(glm::vec4& vector, const std::string& uniformName) const
{
  glUniform4f(GetUniform(uniformName), vector.x, vector.y, vector.z, vector.w);
}

/**
 * Bind matrix value to shader program.
 * @param matrix Value to bind to shader.
 * @param uniformName Name of variable to bind matrix to.
 */
void Shader::Bind(glm::mat4& matrix, const std::string& uniformName) const
{
  glUniformMatrix4fv(GetUniform(uniformName), 1, GL_FALSE, &matrix[0][0]);
}

/**
 * Bind matrix value to shader program.
 * @param matrix Value to bind to shader.
 * @param uniformName Name of variable to bind matrix to.
 */
void Shader::Bind(glm::mat2& matrix, const std::string& uniformName) const
{
  glUniformMatrix2fv(GetUniform(uniformName), 1, GL_FALSE, &matrix[0][0]);
}

/**
 * Bind vector value to shader program.
 * @param vector Value to bind to shader.
 * @param uniformName Name of variable to bind vector to.
 */
void Shader::Bind(std::vector<int>& vector, const std::string& uniformName) const
{
  glUniform1iv(GetUniform(uniformName), static_cast<GLsizei>(vector.size()), vector.data());
}

/**
 * Find errors that occurred during shader compilation.
 * @param handle Handle of the shader to check.
 * @param filePath Path of the shader file for debugging.
 */
void check_shader_error(const GLuint handle, const std::string& filePath)
{
  int isCompiled;
  glGetShaderiv(handle, GL_COMPILE_STATUS, &isCompiled);
  if(isCompiled == GL_FALSE)
  {
    int infoLength;
    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &infoLength);

    std::vector<GLchar> errorMessage(infoLength);
    glGetShaderInfoLog(handle, infoLength, &infoLength, errorMessage.data());

    std::stringstream message;
    message << "Shader error in " << filePath << ": " << errorMessage.data();
    Logger::Error(message.str());
  }
}

/**
 * Find errors that occurred during program linking.
 * @param handle Handle of the program to check.
 * @param vertexPath Path of the loaded vertex shader for debugging.
 * @param fragmentPath Path of the loaded fragment shader for debugging.
 */
void check_program_error(const GLuint handle, const std::string& vertexPath, const std::string& fragmentPath)
{
  int isLinked;
  glGetProgramiv(handle, GL_LINK_STATUS, &isLinked);
  if(isLinked == GL_FALSE)
  {
    int infoLength;
    glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &infoLength);

    std::vector<GLchar> errorMessage(infoLength);
    glGetProgramInfoLog(handle, infoLength, &infoLength, errorMessage.data());

    std::stringstream message;
    message << "Error while linking " << vertexPath << " and " << fragmentPath << ": " << errorMessage.data();
    Logger::Error(message.str());
  }
}

/**
 * Load a shader program from files.
 * @param vertexFilePath Path of vertex program to load.
 * @param fragmentFilePath Path of fragment program to load.
 */
Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath)
{
  std::ifstream vertexStream(vertexFilePath);
  std::stringstream vertexBuffer;
  vertexBuffer << vertexStream.rdbuf();
  auto vertexString = vertexBuffer.str();
  auto vertexSource = vertexString.c_str();

  const auto vertexHandle = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexHandle, 1, &vertexSource, nullptr);
  glCompileShader(vertexHandle);
  check_shader_error(vertexHandle, std::string(vertexFilePath));

  std::ifstream fragmentStream(fragmentFilePath);
  std::stringstream fragmentBuffer;
  fragmentBuffer << fragmentStream.rdbuf();
  auto fragmentString = fragmentBuffer.str();
  auto fragmentSource = fragmentString.c_str();

  const auto fragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentHandle, 1, &fragmentSource, nullptr);
  glCompileShader(fragmentHandle);
  check_shader_error(fragmentHandle, std::string(fragmentFilePath));

  handle = glCreateProgram();
  glAttachShader(handle, vertexHandle);
  glAttachShader(handle, fragmentHandle);

  /*
   * WebGL 1.0 has not support for location statements on attributes.
   * Instead we have to bind them explictly.
   * Mobile Safari misbehaves if that attribute does not exist.
   * We end up with this hacky way of only binding the attribute if it exists.
   * Better not comment those declaration out^^
   */
  if (vertexString.find("attribute vec3 Pos;") != std::string::npos)
    glBindAttribLocation(handle, 0, "Pos");
  if (vertexString.find("attribute vec2 TexCoord;") != std::string::npos)
    glBindAttribLocation(handle, 1, "TexCoord");
  if (vertexString.find("attribute vec3 Normal;") != std::string::npos)
    glBindAttribLocation(handle, 2, "Normal");

  glLinkProgram(handle);
  check_program_error(handle, std::string(vertexFilePath), std::string(fragmentFilePath));

  glDetachShader(handle, vertexHandle);
  glDetachShader(handle, fragmentHandle);

  glDeleteShader(vertexHandle);
  glDeleteShader(fragmentHandle);
}

std::unordered_map<std::string, Shader*> Shader::shaders_;
