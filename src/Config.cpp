#include <emscripten/bind.h>
#include "Config.h"

/*
 *  Default values. Change them as needed.
 */

int Config::geo_resolution_horizontal = 50;
int Config::geo_resolution_vertical = 50;
bool Config::geo_changed = false;

int Config::shadow_resolution = 8000;

int Config::viewport_width = 1;
int Config::viewport_height = 1;

bool Config::ai = true;
float Config::ai_click_delay = 500;
float Config::ai_move_delay = 1000;

bool Config::change_theme = false;
bool Config::new_game = false;
bool Config::undo_turn = false;
bool Config::demo = false;

EMSCRIPTEN_BINDINGS(static_member)
{
  emscripten::class_<Config>("Config")
    .class_property("geo_resolution_horizontal", &Config::geo_resolution_horizontal)
    .class_property("geo_resolution_vertical", &Config::geo_resolution_vertical)
    .class_property("geo_changed", &Config::geo_changed)
    .class_property("ai", &Config::ai)
    .class_property("change_theme", &Config::change_theme)
    .class_property("new_game", &Config::new_game)
    .class_property("undo_turn", &Config::undo_turn)
    .class_property("demo", &Config::demo);
}
