#include <vector>
#include <lodepng.h>
#include <src/Logger.h>
#include "Image.h"

Image::Image(const std::string& path) : width_(), height_()
{
  Load(path);
}

void Image::Load(const std::string& path)
{
  std::vector<unsigned char> buffer;
  lodepng::load_file(buffer, path);
  lodepng::State state;
  state.decoder.color_convert = 0;
  state.decoder.remember_unknown_chunks = 1;
  data_.clear();
  const auto error = decode(data_, width_, height_, state, buffer);
  if (error != 0)
  {
    Logger::Error("Failed to load png: " + std::string(lodepng_error_text(error)));
  }
}

void* Image::GetData()
{
  return data_.data();
}

unsigned int Image::GetWidth() const
{
  return width_;
}

unsigned int Image::GetHeight() const
{
  return height_;
}
