#ifndef CONFIG_H
#define CONFIG_H

class Config
{
public:

  static int geo_resolution_horizontal, geo_resolution_vertical;

  static bool geo_changed;

  static bool ai;

  static float ai_click_delay, ai_move_delay;

  static int viewport_width, viewport_height;

  static int windowed_width, windowed_height;

  static bool change_theme, new_game, undo_turn, demo;

  static bool full_screen;
};

#endif // CONFIG_H
