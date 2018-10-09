#include "image.hpp"
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <lodepng.h>
#include <src/logger.h>

Image::Image(std::string path)
{
    load(path);
}

void Image::load(std::string path){
    std::vector<unsigned char> buffer;
    lodepng::load_file(buffer, path);
    lodepng::State state;
    state.decoder.color_convert = 0;
    state.decoder.remember_unknown_chunks = 1;
    data_.clear();
    auto error = lodepng::decode(data_, width_, height_, state, buffer);
    if(error != 0) {
        Logger::error("Failed to load png: " + std::string(lodepng_error_text(error)));
    }
}
void *Image::getData()
{
    return data_.data();
}

unsigned int Image::getWidth() const
{
    return width_;
}

unsigned int Image::getHeight() const
{
    return height_;
}
