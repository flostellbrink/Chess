#include <vector>
#include <src/Logger.h>
#include "Image.h"
#include <png.h>

Image::Image(const std::string& path) : width_(), height_()
{
  Load(path);
}

void Image::Load(const std::string& path)
{
  FILE *fp = fopen(path.c_str(), "rb");
  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop info = png_create_info_struct(png);
  png_init_io(png, fp);
  png_read_info(png, info);

  width_ = png_get_image_width(png, info);
  height_ = png_get_image_height(png, info);

  data_.resize(png_get_rowbytes(png, info) * height_);
  png_bytep *row_pointers = new png_bytep[height_];
  for (int y = 0; y < height_; y++)
  {
    row_pointers[y] = data_.data() + y * png_get_rowbytes(png, info);
  }

  png_read_image(png, row_pointers);
  delete[] row_pointers;
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
