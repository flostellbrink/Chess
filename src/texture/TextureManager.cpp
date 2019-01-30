#include <utility>

#include <GL/glew.h>

#include "TextureManager.h"
#include "src/Objects.h"
#include <iostream>
#include <src/Logger.h>
#include "Texture.h"


TextureManager::TextureManager(): theme_(0)
{
}

bool TextureManager::IsReflected(const int object)
{
  return object >= objects::white_king && object <= objects::black_pawn
    || object >= objects::clock_base && object <= objects::clock_marks;
}

bool TextureManager::IsTranslucent(const int object) const
{
  return theme_ == themes::glass && object >= objects::white_king && object <= objects::black_pawn;
}

std::vector<Texture*> TextureManager::GetTexture(const int object)
{
  if (texture_collections_[theme_].count(object))
    return texture_collections_[theme_][object];
  return LoadTexture(object);
}

float TextureManager::Reflectivity(const int object) const
{
  switch (object)
  {
  case objects::clock_button:
  case objects::clock_button_border:
  case objects::clock_glass:
  case objects::clock_glass_border:
  case objects::clock_face:
  case objects::clock_leg:
  case objects::clock_pointers:
    return 1;
  default:
    break;
  }

  const auto isGlassPiece = theme_ == themes::glass && object >= objects::white_king && object <= objects::black_pawn;
  return isGlassPiece ? 0.8f : 0.1f;
}

float TextureManager::Shininess(const int object) const
{
  switch (object)
  {
  case objects::clock_button:
  case objects::clock_button_border:
  case objects::clock_glass:
  case objects::clock_glass_border:
    return 15;
  default: break;
  }

  const auto isGlassPiece = theme_ == themes::glass && object >= objects::white_king && object <= objects::black_pawn;
  return isGlassPiece ? 10.0f : 3.0f;
}

std::vector<Texture*> TextureManager::LoadTexture(const int object)
{
  switch (object)
  {
  case objects::table:
    return StoreTexture(object, "res/images/Melamine-wood-005.png");
  case objects::skybox:
    return StoreTexture(object, "res/images/cubemap/Cube", GL_TEXTURE_CUBE_MAP);
  case objects::clock_base:
    return StoreTexture(object, "res/images/Melamine-wood-004.png");
  case objects::clock_face:
    return StoreTexture(object, "res/images/ClockFace.png", GL_TEXTURE_2D);
  case objects::clock_time_cap:
    return StoreTexture(object, "res/images/RedMark.png");
  case objects::clock_marks:
    return StoreTexture(object, "res/images/Melamine-wood-006.png");
  case objects::clock_leg:
  case objects::clock_glass_border:
  case objects::clock_button:
  case objects::clock_button_border:
  case objects::white_seconds_hand:
  case objects::black_seconds_hand:
  case objects::white_minutes_hand:
  case objects::black_minutes_hand:
    return StoreTexture(object, "res/images/Metal.png");
  case objects::fullscreen_quad:
    {
      std::vector<Texture*> result;
      result.push_back(GetTexture("res/images/WhiteWins.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE));
      result.push_back(GetTexture("res/images/BlackWins.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE));
      result.push_back(GetTexture("res/images/Draw.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE));
      return result;
    }
  default: break;
  }

  switch (theme_)
  {
  case themes::wood:
    switch (object)
    {
    case objects::board_border_top:
    case objects::board_border_bottom:
      return StoreTexture(object, "res/images/Melamine-wood-004.png");
    default: break;
    }
    if (object >= objects::field00 && object <= objects::field77)
    {
      std::vector<Texture*> result;
      const auto row = (object - objects::field00) / 8;
      const auto column = (object - objects::field00) % 8;
      const auto texture = row % 2 == column % 2
                             ? "res/images/Melamine-wood-004.png"
                             : "res/images/Melamine-wood-001.png";
      result.push_back(GetTexture(texture));
      result.push_back(GetTexture("res/images/OverlayMove.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE));
      result.push_back(GetTexture("res/images/OverlayHit.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE));
      result.push_back(GetTexture("res/images/OverlayMate1.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE));
      result.push_back(GetTexture("res/images/OverlayMate2.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE));

      texture_collections_[theme_][object] = result;
      return result;
    }
    if (object >= objects::white_king && object <= objects::black_pawn)
    {
      const auto texture = object % 2
                             ? "res/images/Melamine-wood-004.png"
                             : "res/images/Melamine-wood-001.png";
      return StoreTexture(object, texture);
    }
    break;
  case themes::glass:
    switch (object)
    {
    case objects::board_border_top:
    case objects::board_border_bottom:
      return StoreTexture(object, "res/images/Melamine-wood-004.png");
    default: break;
    }
    if (object >= objects::field00 && object <= objects::field77)
    {
      std::vector<Texture*> result;
      const auto row = (object - objects::field00) / 8;
      const auto column = (object - objects::field00) % 8;
      const auto texture = row % 2 == column % 2
                             ? "res/images/Melamine-wood-004.png"
                             : "res/images/Melamine-wood-001.png";
      result.push_back(GetTexture(texture));
      result.push_back(GetTexture("res/images/OverlayMove.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE));
      result.push_back(GetTexture("res/images/OverlayHit.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE));
      result.push_back(GetTexture("res/images/OverlayMate1.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE));
      result.push_back(GetTexture("res/images/OverlayMate2.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE)); //broken

      texture_collections_[theme_][object] = result;
      return result;
    }
    if (object >= objects::white_king && object <= objects::black_pawn)
    {
      const auto texture = object % 2
                             ? "res/images/BlackGlass.png"
                             : "res/images/WhiteGlass.png";
      return StoreTexture(object, texture);
    }
    break;
  default: break;
  }
  Logger::Warn("Using default texture for objectID: " + std::to_string(object));
  return StoreTexture(object, "res/images/Melamine-wood-001.png");
}

std::vector<Texture*> TextureManager::StoreTexture(const int object,
                                                   const std::string& filePath,
                                                   const GLenum target,
                                                   const GLenum textureRepeat)
{
  auto result = std::vector<Texture*>{GetTexture(filePath, target, textureRepeat)};
  texture_collections_[theme_][object] = result;
  return result;
}

Texture* TextureManager::GetTexture(const std::string& path, const GLenum target, const GLenum textureRepeat)
{
  if (textures_[theme_].count(path))
    return textures_[theme_][path];
  return LoadTexture(path, target, textureRepeat);
}

Texture* TextureManager::LoadTexture(const std::string& path, const GLenum target, const GLenum textureRepeat)
{
  auto* result = new Texture(path, target, textureRepeat);
  textures_[theme_][path] = result;
  return result;
}

void TextureManager::SetTheme(const int theme)
{
  theme_ = theme;
}
