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

project("Chess" C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -Wextra")
endif()

# Required libraries
find_package(Qt5OpenGL 5.4.0 REQUIRED)

# process .ui files
qt5_wrap_ui(ui_mainwindow.h gui/mainwindow.ui)

#process resource files
set(allRessources
        res/imagesClockface.qrc
        res/imagesGlass0.qrc
        res/imagesGlass1.qrc
        res/imagesMetal.qrc
        res/imagesOver0.qrc
        res/imagesOver1.qrc
        res/imagesOver2.qrc
        res/imagesOver3.qrc
        res/imagesOver4.qrc
        res/imagesOver5.qrc
        res/imagesOver6.qrc
        res/imagesRedMark.qrc
        res/imagesSky0.qrc
        res/imagesSky1.qrc
        res/imagesSky2.qrc
        res/imagesSky3.qrc
        res/imagesSky4.qrc
        res/imagesSky5.qrc
        res/imagesWood0.qrc
        res/imagesWood1.qrc
        res/imagesWood2.qrc
        res/imagesWood3.qrc
        res/imagesWood4.qrc
        res/imagesWood5.qrc
        res/shader.qrc)

qt5_add_resources (ResourceSources ${allRessources})

# Optional libraries
find_package(GTA QUIET)

# The utility library
add_subdirectory(glbase)

# The program
set(sourceFiles
        # Special
        main.cpp
        objects.hpp
        gui/mainwindow.ui

        # Graphical user interface
        gui/mainwindow.hpp
        gui/mainwindow.cpp
        gui/glwidget.hpp
        gui/glwidget.cpp
        gui/config.h
        gui/config.cpp

        # Camera
        camera.hpp
        camera.cpp

        # Objects
        objects/objectManager.hpp
        objects/objectManager.cpp
        objects/drawable.hpp
        objects/drawable.cpp
        objects/basicObject.hpp
        objects/basicObject.cpp
        objects/fullscreenQuad.hpp
        objects/fullscreenQuad.cpp
        objects/board.hpp
        objects/board.cpp
        objects/field.hpp
        objects/field.cpp
        objects/piece.hpp
        objects/piece.cpp
        objects/pieces/bishop.hpp
        objects/pieces/bishop.cpp
        objects/pieces/king.hpp
        objects/pieces/king.cpp
        objects/pieces/knight.hpp
        objects/pieces/knight.cpp
        objects/pieces/pawn.hpp
        objects/pieces/pawn.cpp
        objects/pieces/queen.hpp
        objects/pieces/queen.cpp
        objects/pieces/rook.hpp
        objects/pieces/rook.cpp
        objects/clock.hpp
        objects/clock.cpp
        objects/skybox.hpp
        objects/skybox.cpp

        # Chess Moves
        objects/moves/moveBase.hpp
        objects/moves/moveBase.cpp
        objects/moves/move.hpp
        objects/moves/move.cpp
        objects/moves/hit.hpp
        objects/moves/hit.cpp
        objects/moves/rochade.hpp
        objects/moves/rochade.cpp
        objects/moves/transform.hpp
        objects/moves/transform.cpp

        # Collision detection
        collision/collManager.hpp
        collision/collManager.cpp
        collision/collision.hpp
        collision/collision.cpp

        # Geometry
        geometry/geoManager.hpp
        geometry/geoManager.cpp
        geometry/controlPoints.hpp
        geometry/controlPoints.cpp
        geometry/geometry.hpp
        geometry/geometry.cpp
        geometry/compositeGeometry.hpp
        geometry/compositeGeometry.cpp
        geometry/implementation/boardBorderBottom.hpp
        geometry/implementation/boardBorderBottom.cpp
        geometry/implementation/boardBorderTop.hpp
        geometry/implementation/boardBorderTop.cpp
        geometry/implementation/cube.hpp
        geometry/implementation/cube.cpp
        geometry/implementation/extruder.hpp
        geometry/implementation/extruder.cpp
        geometry/implementation/extruderFace.hpp
        geometry/implementation/extruderFace.cpp
        geometry/implementation/interlacedRevolver.hpp
        geometry/implementation/interlacedRevolver.cpp
        geometry/implementation/quad.hpp
        geometry/implementation/quad.cpp
        geometry/implementation/revolver.hpp
        geometry/implementation/revolver.cpp
        geometry/implementation/square.hpp
        geometry/implementation/square.cpp

        # Curves
        curve/curve.hpp
        curve/curve.cpp
        curve/catmullRomCurve.hpp
        curve/catmullRomCurve.cpp
        curve/circleCurve.hpp
        curve/circleCurve.cpp
        curve/fixedPointsCurve.hpp
        curve/fixedPointsCurve.cpp
        curve/lineCurve.hpp
        curve/lineCurve.cpp

        # Animation
        animation/aniManager.hpp
        animation/aniManager.cpp
        animation/animationBase.hpp
        animation/animation.hpp
        animation/animation.cpp
        animation/loopingAnimation.hpp
        animation/loopingAnimation.cpp
        animation/parallelAnimation.hpp
        animation/parallelAnimation.cpp
        animation/serialAnimation.hpp
        animation/serialAnimation.cpp
        animation/backLoopingAnimation.hpp
        animation/backLoopingAnimation.cpp
        animation/catmullRomAnimation.hpp
        animation/fadeAnimation.hpp
        animation/fadeAnimation.cpp
        animation/linearAnimation.hpp
        animation/linearAnimation.cpp

        # Textures
        texture/texManager.hpp
        texture/texManager.cpp
        texture/image.hpp
        texture/image.cpp
        texture/texturewrapper.hpp
        texture/texturewrapper.cpp
)

add_executable(chess
    ${sourceFiles}
    ${ResourceSources}
)

include_directories(${CMAKE_SOURCE_DIR}/glbase ${OPENGL_INCLUDE_DIR})

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
