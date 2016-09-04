#ifndef CONFIG_H
#define CONFIG_H
#include <qstring.h>

/**
 * @brief The global configuration
 *
 * This class contains global configuration parameters
 * that are used all over the program.
 */
class Config{
public:

    static int geoResolution1, geoResolution2, shadowResolution;

    static bool ai;

    static bool changed;

    static int viewportWidth, viewportHeight;

    static bool changeTheme, newGame, undoTurn, demo;
};

#endif // CONFIG_H

