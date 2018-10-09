#ifndef TEXMANAGER_H
#define TEXMANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include "texturewrapper.hpp"

using std::vector;
using std::map;
using std::string;

class TexManager{
    public:
    TexManager();
    /**
     * @brief Texture Get Textures for Object, if they are not loaded yet load them
     * @param object The Objects ID
     * @return Textures
     */
    vector<TextureWrapper*> Texture(int object);
    /**
     * @brief SetTheme Sets a new theme, removing the already loaded texture collection and recreating them on demand
     * @param theme The new Themes ID
     */
    void SetTheme(int theme);

    bool IsTranslucent(int object);
    bool IsReflected(int object);

    float reflectivity(int object);
    float shininess(int object);

    protected:
    /**
     * @brief loadTexture Creates a collection for all Textures used by an object
     * @param object The Objects ID
     * @return Textures
     */
    vector<TextureWrapper*> loadTexture(int object);
    vector<TextureWrapper*> storeTexture(int object, string filePath, GLenum target = GL_TEXTURE_2D, GLenum textureRepeat = GL_REPEAT);
    /**
     * @brief Texture Get a Texture for a path
     * @param path The textures filepath
     * @param target Rendertarget (for skybox)
     * @return Texture
     */
    TextureWrapper* Texture(string path, GLenum target = GL_TEXTURE_2D, GLenum textureRepeat = GL_REPEAT);
    /**
     * @brief loadTexture Loads a texture from file
     * @param path The filepath to load from
     * @param target Rendertarget (for skybox)
     * @return Texture
     */
    TextureWrapper* loadTexture(string path, GLenum target = GL_TEXTURE_2D, GLenum textureRepeat = GL_REPEAT);

    private:
    /**
     * @brief _textureCollections stores collections of textures for all objects, for each theme
     */
    map<int, map<int, vector<TextureWrapper*>>> _textureCollections;
    /**
     * @brief _textures stores textures and the related path, for each theme
     */
    map<int, map<string, TextureWrapper*>> _textures;
    int _theme;
};


#endif // TEXMANAGER_H
