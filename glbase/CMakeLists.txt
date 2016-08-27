# Copyright (C) 2013, 2014
# Computer Graphics Group, University of Siegen
# Written by Martin Lambers <martin.lambers@uni-siegen.de>
#
# Copying and distribution of this file, with or without modification, are
# permitted in any medium without royalty provided the copyright notice and this
# notice are preserved. This file is offered as-is, without any warranty.

cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
SET(CMAKE_BUILD_TYPE Debug)
cmake_policy(SET CMP0017 NEW)

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -Wextra")
endif()

# Optional libraries
find_package(GTA QUIET)

# The library
include_directories(${CMAKE_SOURCE_DIR})
if(GTA_FOUND)
	add_definitions(-DHAVE_GTA)
	include_directories(${GTA_INCLUDE_DIR})
endif()
add_library(libglbase STATIC
	gltool.hpp gltool.cpp
	navigator.hpp navigator.cpp
	geometries.hpp geometries.cpp
        texload.hpp texload.cpp
        geomload.hpp geomload.cpp
	lodepng.h lodepng.cpp
	ply.h plyfile.cpp
	tiny_obj_loader.h tiny_obj_loader.cc
	glew.c GL/glew.h GL/glxew.h GL/wglew.h)
set_target_properties(libglbase PROPERTIES OUTPUT_NAME glbase)
if(WIN32 OR CYGWIN)
        target_link_libraries(libglbase opengl32)
endif()
install(TARGETS libglbase RUNTIME DESTINATION bin LIBRARY DESTINATION "lib" ARCHIVE DESTINATION "lib")
