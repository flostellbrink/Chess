#include "Config.h"

/*
 *  Default values. Change them as needed.
 */

int Config::geo_resolution1 = 50,
Config::geo_resolution2 = 50,
Config::shadow_resolution = 8000;

int Config::viewport_width = 1,
Config::viewport_height = 1;

bool Config::ai = false;

bool Config::change_theme = false;
bool Config::new_game = false;
bool Config::undo_turn = false;
bool Config::demo = false;

bool Config::changed = false;

bool Config::full_screen = false;
