#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/glm.hpp>
#include <vector>

/**
 * Holds a shader program.
 */
class Shader {
public:
  Shader(const std::string & vertexFilePath, const std::string & fragmentFilePath);
  GLuint handle;
  void Use() const;
  GLint GetUniform(const std::string &uniformName) const;
  bool HasUniform(const std::string &uniformName) const;
  void Bind(int scalar, const std::string &uniformName) const;
  void Bind(float scalar, const std::string &uniformName) const;
  void Bind(glm::vec2 &vector, const std::string &uniformName) const;
  void Bind(glm::vec3 &vector, const std::string &uniformName) const;
  void Bind(glm::vec4 &vector, const std::string &uniformName) const;
  void Bind(glm::mat2 &matrix, const std::string &uniformName) const;
  void Bind(glm::mat4 &matrix, const std::string &uniformName) const;
  void Bind(std::vector<int> &vector, const std::string &uniformName) const;
};

#endif //SHADER_HPP
