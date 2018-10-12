#include "Config.h"

/*
 *  Default values. Change them as needed.
 */

int Config::geo_resolution1 = 50;
int Config::geo_resolution2 = 50;
int Config::shadow_resolution = 8000;

int Config::viewport_width = 1;
int Config::viewport_height = 1;

int Config::windowed_width = 1920;
int Config::windowed_height = 1080;

bool Config::ai = true;
float Config::ai_click_delay = 500;
float Config::ai_move_delay = 1000;

bool Config::change_theme = false;
bool Config::new_game = false;
bool Config::undo_turn = false;
bool Config::demo = false;

bool Config::changed = false;

bool Config::full_screen = false;
