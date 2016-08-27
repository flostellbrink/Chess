#ifndef BITMAP_H
#define BITMAP_H

#include <GL/glew.h>
#include <string>

#include <QImage>

/**
 * @brief The Image class is a wrapper to use images as textures
 *
 * You can use this class to load textures and send them
 * to your shader. The image will be present in
 * 32-bit RGBA format (4 * 8 bit)
 */
class Image
{
public:

    /**
     * @brief Image constructor
     * @param path The path of the image
     *
     * You can use the Qt Ressource system for the path,
     * e.g. ":/res/images/earth.bmp"
     */
    Image(std::string path);

    /**
     * @brief getWidth Getter for the image width
     * @return the width of the image in pixels
     */
    unsigned int getWidth() const;

    /**
     * @brief getHeight Getter for the image height
     * @return the height of the image in pixels
     */
    unsigned int getHeight() const;

    /**
     * @brief getData Getter for the image data
     * @return the image data as uchar array
     *
     * You can pass this data directly to the shader
     */
    uchar *getData();

private:
    /**
     * @brief load Loads the image
     * @param path the path to the image
     */
    void load(std::string path);

    QImage _image; /**< The image in the QImage format */
};

#endif // BITMAP_H
