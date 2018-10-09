#include "config.h"

/*
 *  Default values. Change them as needed.
 */

int Config::geoResolution1 = 50,
    Config::geoResolution2 = 50,
    Config::shadowResolution = 8000;

int Config::viewportWidth = 1,
    Config::viewportHeight = 1;

bool Config::ai = false;

bool Config::changeTheme = false;
bool Config::newGame = false;
bool Config::undoTurn = false;
bool Config::demo = false;

bool Config::changed = false;
