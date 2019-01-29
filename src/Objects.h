// Contains an enum for every object, used for textures and geometry

#ifndef OBJECTS_H
#define OBJECTS_H

namespace objects
{
  const int
    board = 0,
    skybox = 1,
    field00 = 2,
    field10 = 3,
    // Field 11 to Field 67 implicit
    field77 = 65,
    white_king = 66,
    black_king = 67,
    white_queen = 68,
    black_queen = 69,
    white_rook = 70,
    black_rook = 71,
    white_bishop = 72,
    black_bishop = 73,
    white_knight = 74,
    black_knight = 75,
    white_pawn = 76,
    black_pawn = 77,
    table = 78,
    board_border_top = 79,
    board_border_bottom = 80,
    clock_base = 81,
    clock_leg = 82,
    clock_glass = 83,
    clock_button = 84,
    clock_pointers = 85,
    clock_glass_border = 86,
    clock_button_border = 87,
    clock_face = 88,
    white_minutes_hand = 89,
    white_seconds_hand = 90,
    black_minutes_hand = 91,
    black_seconds_hand = 92,
    clock_time_cap = 93,
    clock_marks = 94,
    fullscreen_quad = 95;
}

namespace themes
{
  const unsigned int
    wood = 0,
    metal = 1,
    glass = 2;
}

#endif // OBJECTS_H
