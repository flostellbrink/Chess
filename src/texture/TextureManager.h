#ifndef TEX_MANAGER_H
#define TEX_MANAGER_H

#include <GL/glew.h>
#include <vector>
#include <map>

class Texture;

class TextureManager
{
public:
  TextureManager();
  /**
   * @brief Texture Get Textures for Object, if they are not loaded yet load them
   * @param object The Objects ID
   * @return Textures
   */
  std::vector<Texture*> GetTexture(int object);
  /**
   * @brief SetTheme Sets a new theme, removing the already loaded texture collection and recreating them on demand
   * @param theme The new Themes ID
   */
  void SetTheme(int theme);

  bool IsTranslucent(int object) const;
  static bool IsReflected(int object);

  float Reflectivity(int object) const;
  float Shininess(int object) const;

protected:
  /**
   * @brief loadTexture Creates a collection for all Textures used by an object
   * @param object The Objects ID
   * @return Textures
   */
  std::vector<Texture*> LoadTexture(int object);
  std::vector<Texture*> StoreTexture(int object,
                                     const std::string& filePath,
                                     GLenum target = GL_TEXTURE_2D,
                                     GLenum textureRepeat = GL_REPEAT);
  /**
   * @brief Texture Get a Texture for a path
   * @param path The textures filepath
   * @param target Render target (for sky box)
   * @param textureRepeat Repeat mode of texture
   * @return Texture
   */
  Texture* GetTexture(const std::string& path, GLenum target = GL_TEXTURE_2D, GLenum textureRepeat = GL_REPEAT);
  /**
   * @brief loadTexture Loads a texture from file
   * @param path The filepath to load from
   * @param target Render target (for sky box)
   * @param textureRepeat Repeat mode of texture
   * @return Texture
   */
  Texture* LoadTexture(const std::string& path, GLenum target = GL_TEXTURE_2D, GLenum textureRepeat = GL_REPEAT);

private:
  /**
   * @brief _textureCollections stores collections of textures for all objects, for each theme
   */
  std::map<int, std::map<int, std::vector<Texture*>>> texture_collections_;
  /**
   * @brief _textures stores textures and the related path, for each theme
   */
  std::map<int, std::map<std::string, Texture*>> textures_;
  int theme_;
};


#endif // TEX_MANAGER_H
