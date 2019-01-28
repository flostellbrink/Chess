# lodepng
add_library(lodepng STATIC lodepng/lodepng.cpp lodepng/lodepng.h)
target_include_directories(lodepng PRIVATE lodepng)
