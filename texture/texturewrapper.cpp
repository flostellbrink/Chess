#include <GL/glew.h>
#include "texturewrapper.hpp"
#include <texture/image.hpp>
#include "glbase/gltool.hpp"
#include <iostream>

TextureWrapper::TextureWrapper(){

}

TextureWrapper::TextureWrapper(const std::string& texturePath, GLenum target, GLenum repeatMode)
{
    _textureHandle = 0;
    glGenTextures(1, &_textureHandle);
    glBindTexture(target, _textureHandle);
    VERIFY(CG::checkError());

    if(target == GL_TEXTURE_CUBE_MAP){
        for(GLuint i = 0; i < 6; i++){
            Image image = Image(texturePath + std::to_string(i) + ".jpg");
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getData());
            VERIFY(CG::checkError());
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        VERIFY(CG::checkError());
    } else {
        Image image = Image(texturePath);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getData());
        VERIFY(CG::checkError());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatMode);

        VERIFY(CG::checkError());

        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        VERIFY(CG::checkError());
    }
}

void TextureWrapper::Bind(GLenum target, unsigned int textureNum) const {
    //TODO: expand to support binding multiple textures
    glActiveTexture(GL_TEXTURE0 + textureNum);
    glBindTexture(target, _textureHandle);
}

TextureWrapper::~TextureWrapper(){
    //glDeleteTextures(1, &_textureHandle);
}

