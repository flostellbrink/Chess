#include <GL/glew.h>
#ifndef TEXTUREWRAPPER_H
#define TEXTUREWRAPPER_H
#include <string>

class TextureWrapper
{
public:
    TextureWrapper();
    TextureWrapper(const std::string& texturePath, GLenum target = GL_TEXTURE_2D, GLenum repeatMode = GL_REPEAT);

    void Bind(GLenum target = GL_TEXTURE_2D, unsigned int textureNum = 0) const;

    ~TextureWrapper();
private:
    GLuint _textureHandle;
};

#endif // TEXTUREWRAPPER_H
