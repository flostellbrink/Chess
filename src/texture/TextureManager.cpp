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
  return object >= objects::white_king && object <= objects::black_pawn;
}

bool TextureManager::IsTranslucent(const int object) const
{
  return theme_ == themes::glass && object >= objects::white_king && object <= objects::black_pawn;
}

std::vector<Texture*> TextureManager::GetTexture(const int object) {
  if (textureCollections_[theme_].count(object))
    return textureCollections_[theme_][object];
  return LoadTexture(object);
}

float TextureManager::Reflectivity(const int object) const
{
  switch (object) {
  case objects::clock_button:
  case objects::clock_button_border:
  case objects::clock_glass:
  case objects::clock_glass_border:
  case objects::clock_face:
    return 1;
  default:
    break;
  }

  const auto isGlassPiece = theme_ == themes::glass && object >= objects::white_king && object <= objects::black_pawn;
  return isGlassPiece ? 0.8f : 0.1f;
}

float TextureManager::Shininess(int object) const
{
  switch (object) {
  case objects::clock_button:
  case objects::clock_button_border:
  case objects::clock_glass:
  case objects::clock_glass_border:
    return 15;
  default:break;
  }

  const auto isGlassPiece = theme_ == themes::glass && object >= objects::white_king && object <= objects::black_pawn;
  return isGlassPiece ? 10.0f : 3.0f;
}

std::vector<Texture*> TextureManager::LoadTexture(int object) {
  switch (object) {
  case objects::table:
    return storeTexture(object, "res/images/Melamine-wood-005.png");
  case objects::skybox:
    return storeTexture(object, "res/images/cubemap/Cube", GL_TEXTURE_CUBE_MAP);
  case objects::clock_base:
    return storeTexture(object, "res/images/Melamine-wood-004.png");
  case objects::clock_face:
    return storeTexture(object, "res/images/ClockFace.png", GL_TEXTURE_2D);
  case objects::clock_time_cap:
    return storeTexture(object, "res/images/RedMark.png");
  case objects::clock_marks:
    return storeTexture(object, "res/images/Melamine-wood-006.png");
  case objects::clock_leg:
  case objects::clock_glass_border:
  case objects::clock_button:
  case objects::clock_button_border:
  case objects::white_seconds_hand:
  case objects::black_seconds_hand:
  case objects::white_minutes_hand:
  case objects::black_minutes_hand:
    return storeTexture(object, "res/images/Metal.png");
  case objects::fullscreen_quad: {
    std::vector<Texture*> result;
    result.push_back(GetTexture("res/images/WhiteWins.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE));
    result.push_back(GetTexture("res/images/BlackWins.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE));
    result.push_back(GetTexture("res/images/Draw.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE));
    return result;
  }
  default:break;
  }

  switch (theme_) {
  case themes::wood:
    switch (object) {
    case objects::board_border_top:
    case objects::board_border_bottom:
      return storeTexture(object, "res/images/Melamine-wood-004.png");
    default:break;
    }
    if (object >= objects::field00 && (object <= objects::field77)) {
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

      textureCollections_[theme_][object] = result;
      return result;
    }
    if (object >= objects::white_king && object <= objects::black_pawn) {
      const auto texture = object % 2
        ? "res/images/Melamine-wood-004.png"
        : "res/images/Melamine-wood-001.png";
      return storeTexture(object, texture);
    }
    break;
  case themes::glass:
    switch (object) {
    case objects::board_border_top:
    case objects::board_border_bottom:
      return storeTexture(object, "res/images/Melamine-wood-004.png");
    default:break;
    }
    if ((object >= objects::field00) && (object <= objects::field77)) {
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

      textureCollections_[theme_][object] = result;
      return result;
    }
    if ((object >= objects::white_king && object <= objects::black_pawn)) {
      const auto texture = object % 2
        ? "res/images/BlackGlass.png"
        : "res/images/WhiteGlass.png";
      return storeTexture(object, texture);
    }
    break;
  default:break;
  }
  Logger::Warn("ChessWarn: Using default texture for objectID: " + std::to_string(object));
  return storeTexture(object, "res/images/Melamine-wood-001.png");
}

std::vector<Texture*> TextureManager::storeTexture(int object, std::string filePath, GLenum target, GLenum textureRepeat) {
  std::vector<Texture*> result = std::vector<Texture*>{ GetTexture(filePath, target, textureRepeat) };
  textureCollections_[theme_][object] = result;
  return result;
}

Texture* TextureManager::GetTexture(const std::string& path, const GLenum target, const GLenum textureRepeat) {
  if (textures_[theme_].count(path))
    return textures_[theme_][path];
  return LoadTexture(path, target, textureRepeat);
}

Texture* TextureManager::LoadTexture(const std::string& path, const GLenum target, const GLenum textureRepeat) {
  auto * result = new Texture(path, target, textureRepeat);
  textures_[theme_][path] = result;
  return result;
}

void TextureManager::SetTheme(int theme) {
  theme_ = theme;
}
