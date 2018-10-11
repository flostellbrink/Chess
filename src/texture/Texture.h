#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>

class Texture
{
public:
  Texture();
  explicit Texture(const std::string& texturePath, GLenum target = GL_TEXTURE_2D, GLenum repeatMode = GL_REPEAT);
  void Bind(GLenum target = GL_TEXTURE_2D, unsigned int textureNum = 0) const;
  ~Texture();

private:
  GLuint texture_handle_;
};

#endif // TEXTURE_H
