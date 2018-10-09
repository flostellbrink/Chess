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
    std::vector<unsigned char> buffer, image_flipped, image;
    lodepng::load_file(buffer, path);
    unsigned width, height;
    lodepng::State state;
    state.decoder.color_convert = 0;
    state.decoder.remember_unknown_chunks = 1;
    auto error = lodepng::decode(image_flipped, width, height, state, buffer);
    if(error != 0) {
        Logger::error("Failed to load png: " + std::string(lodepng_error_text(error)));
    }

    // png has origin in lower left, opengl in upper left, so need to flip
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width * 4; col++) {
            image.push_back(image_flipped[(height - 1 - row) * width * 4 + col]);
        }
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
