#include "image.hpp"
#include <GL/glew.h>

#include <iostream>

#include <QFile>

Image::Image(std::string path)
{
    load(path);
}

void Image::load(std::string path){

    _image = QImage(path.c_str()).convertToFormat(QImage::Format_RGBA8888);
}
uchar *Image::getData()
{
    return _image.bits();
}

unsigned int Image::getHeight() const
{
    return _image.height();
}

unsigned int Image::getWidth() const
{
    return _image.width();
}


