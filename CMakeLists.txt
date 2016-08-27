# Copyright (C) 2013, 2014
# Computer Graphics Group, University of Siegen
# Written by Martin Lambers <martin.lambers@uni-siegen.de>
#
# Copying and distribution of this file, with or without modification, are
# permitted in any medium without royalty provided the copyright notice and this
# notice are preserved. This file is offered as-is, without any warranty.

cmake_minimum_required(VERSION 2.8.6)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
cmake_policy(SET CMP0017 NEW)
cmake_policy(SET CMP0020 NEW)

project("GraPra 2015 - Tycho Brahe" C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

#if(CMAKE_COMPILER_IS_GNUCXX)
#	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -Wextra")
#endif()

# Required libraries
find_package(Qt5OpenGL 5.4.0 REQUIRED)

# process .ui files
############################
# List your ui files here  #
# (if you need any)        #
############################
qt5_wrap_ui(ui_mainwindow.h gui/mainwindow.ui)

#process resource files
############################
# List your qrc files here #
# (if you need any)        #
############################
file(GLOB_RECURSE allRessources "*.qrc" )
qt5_add_resources (ResourceSources ${allRessources})

# Optional libraries
find_package(GTA QUIET)

# The utility library
add_subdirectory(glbase)

# The program
################################
# List your project files here #
################################
file(GLOB_RECURSE sourceFiles
    "*.h"
    "*.hpp"
    "*.cpp"
    "*.glsl"
    "*.ui"
)
add_executable(chess
    ${sourceFiles}
    ${ResourceSources}
)

include_directories(${CMAKE_SOURCE_DIR}/glbase ${OPENGL_INCLUDE_DIR})

####################################
# Build with correct opengl lib    #
# (this could fail on your system) #
####################################
if(WIN32 OR CYGWIN)
        target_link_libraries(chess opengl32 libglbase Qt5::OpenGL ${OPENGL_gl_LIBRARY})
else()
        target_link_libraries(chess GL libglbase Qt5::OpenGL ${OPENGL_gl_LIBRARY})
endif()

if(GTA_FOUND)
        add_definitions(-DHAVE_GTA)
        include_directories(${GTA_INCLUDE_DIR})
        target_link_libraries(chess ${GTA_LIBRARIES})
endif()

# copy images
add_custom_command(TARGET chess POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/images $<TARGET_FILE_DIR:chess>/images)

install(TARGETS chess RUNTIME DESTINATION bin)
