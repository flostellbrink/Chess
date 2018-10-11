#include <GL/glew.h>
#include "Texture.h"
#include <src/texture/Image.h>

Texture::Texture() : texture_handle_()
{

}

Texture::Texture(const std::string& texturePath, GLenum target, GLenum repeatMode)
{
  texture_handle_ = 0;
  glGenTextures(1, &texture_handle_);
  glBindTexture(target, texture_handle_);

  if (target == GL_TEXTURE_CUBE_MAP) {
    for (GLuint i = 0; i < 6; i++) {
      auto image = Image(texturePath + std::to_string(i) + ".png");
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getData());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }
  else {
    auto image = Image(texturePath);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getData());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatMode);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
}

void Texture::Bind(GLenum target, unsigned int textureNum) const {
  glActiveTexture(GL_TEXTURE0 + textureNum);
  glBindTexture(target, texture_handle_);
}

Texture::~Texture() {
  glDeleteTextures(1, &texture_handle_);
}

