#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>

/**
 * Holds a shader program.
 */
class Shader
{
public:
  static Shader* GetShaderCached(const std::string& vertexFilePath, const std::string& fragmentFilePath);
  GLuint handle;
  void Use() const;
  GLint GetUniform(const std::string& uniformName) const;
  bool HasUniform(const std::string& uniformName) const;
  void Bind(int scalar, const std::string& uniformName) const;
  void Bind(float scalar, const std::string& uniformName) const;
  void Bind(glm::vec2& vector, const std::string& uniformName) const;
  void Bind(glm::vec3& vector, const std::string& uniformName) const;
  void Bind(glm::vec4& vector, const std::string& uniformName) const;
  void Bind(glm::mat2& matrix, const std::string& uniformName) const;
  void Bind(glm::mat4& matrix, const std::string& uniformName) const;
  void Bind(std::vector<int>& vector, const std::string& uniformName) const;
private:
  Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
  static std::unordered_map<std::string, Shader*> shaders_;
};

#endif //SHADER_HPP
