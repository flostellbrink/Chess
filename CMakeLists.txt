# CMake entry point
cmake_minimum_required(VERSION 3.9)
set (CMAKE_CXX_STANDARD 14)
set(PROJECT_NAME "Chess")
project(${PROJECT_NAME})

include(conanbuildinfo.cmake)
conan_basic_setup()

find_package(OpenGL REQUIRED)

# Compile non-conan dependencies
add_subdirectory(lib)

include_directories(
        lib/lodepng/
        .
)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/objects.hpp
        src/config.h
        src/config.cpp
        src/objects/camera.hpp
        src/objects/camera.cpp
        src/objects/objectManager.hpp
        src/objects/objectManager.cpp
        src/objects/drawable.hpp
        src/objects/drawable.cpp
        src/objects/basicObject.hpp
        src/objects/basicObject.cpp
        src/objects/fullscreenQuad.hpp
        src/objects/fullscreenQuad.cpp
        src/objects/board.hpp
        src/objects/board.cpp
        src/objects/field.hpp
        src/objects/field.cpp
        src/objects/piece.hpp
        src/objects/piece.cpp
        src/objects/pieces/bishop.hpp
        src/objects/pieces/bishop.cpp
        src/objects/pieces/king.hpp
        src/objects/pieces/king.cpp
        src/objects/pieces/knight.hpp
        src/objects/pieces/knight.cpp
        src/objects/pieces/pawn.hpp
        src/objects/pieces/pawn.cpp
        src/objects/pieces/queen.hpp
        src/objects/pieces/queen.cpp
        src/objects/pieces/rook.hpp
        src/objects/pieces/rook.cpp
        src/objects/clock.hpp
        src/objects/clock.cpp
        src/objects/skybox.hpp
        src/objects/skybox.cpp
        src/objects/moves/moveBase.hpp
        src/objects/moves/moveBase.cpp
        src/objects/moves/move.hpp
        src/objects/moves/move.cpp
        src/objects/moves/hit.hpp
        src/objects/moves/hit.cpp
        src/objects/moves/rochade.hpp
        src/objects/moves/rochade.cpp
        src/objects/moves/transform.hpp
        src/objects/moves/transform.cpp
        src/collision/collManager.hpp
        src/collision/collManager.cpp
        src/collision/collision.hpp
        src/collision/collision.cpp
        src/geometry/geoManager.hpp
        src/geometry/geoManager.cpp
        src/geometry/controlPoints.hpp
        src/geometry/controlPoints.cpp
        src/geometry/geometry.hpp
        src/geometry/geometry.cpp
        src/geometry/compositeGeometry.hpp
        src/geometry/compositeGeometry.cpp
        src/geometry/implementation/boardBorderBottom.hpp
        src/geometry/implementation/boardBorderBottom.cpp
        src/geometry/implementation/boardBorderTop.hpp
        src/geometry/implementation/boardBorderTop.cpp
        src/geometry/implementation/cube.hpp
        src/geometry/implementation/cube.cpp
        src/geometry/implementation/extruder.hpp
        src/geometry/implementation/extruder.cpp
        src/geometry/implementation/extruderFace.hpp
        src/geometry/implementation/extruderFace.cpp
        src/geometry/implementation/interlacedRevolver.hpp
        src/geometry/implementation/interlacedRevolver.cpp
        src/geometry/implementation/quad.hpp
        src/geometry/implementation/quad.cpp
        src/geometry/implementation/revolver.hpp
        src/geometry/implementation/revolver.cpp
        src/geometry/implementation/square.hpp
        src/geometry/implementation/square.cpp
        src/curve/curve.hpp
        src/curve/curve.cpp
        src/curve/catmullRomCurve.hpp
        src/curve/catmullRomCurve.cpp
        src/curve/circleCurve.hpp
        src/curve/circleCurve.cpp
        src/curve/fixedPointsCurve.hpp
        src/curve/fixedPointsCurve.cpp
        src/curve/lineCurve.hpp
        src/curve/lineCurve.cpp
        src/animation/aniManager.hpp
        src/animation/aniManager.cpp
        src/animation/animationBase.hpp
        src/animation/animation.hpp
        src/animation/animation.cpp
        src/animation/loopingAnimation.hpp
        src/animation/loopingAnimation.cpp
        src/animation/parallelAnimation.hpp
        src/animation/parallelAnimation.cpp
        src/animation/serialAnimation.hpp
        src/animation/serialAnimation.cpp
        src/animation/backLoopingAnimation.hpp
        src/animation/backLoopingAnimation.cpp
        src/animation/catmullRomAnimation.hpp
        src/animation/fadeAnimation.hpp
        src/animation/fadeAnimation.cpp
        src/animation/linearAnimation.hpp
        src/animation/linearAnimation.cpp
        src/texture/texManager.hpp
        src/texture/texManager.cpp
        src/texture/image.hpp
        src/texture/image.cpp
        src/texture/texturewrapper.hpp
        src/texture/texturewrapper.cpp
        )

# Copy shaders to build directory
add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res
        COMMENT "Copy Resources")

add_dependencies(${PROJECT_NAME} copy_resources)

# Setup installer
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
install(DIRECTORY res
        DESTINATION ./)

SET(CPACK_PACKAGE_NAME "Chess")
SET(CPACK_PACKAGE_VERSION_MAJOR 1)
SET(CPACK_PACKAGE_VERSION_MINOR 0)
SET(CPACK_PACKAGE_VERSION_PATCH 0)
SET(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Chess Game")
SET(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)
SET(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME};Chess")
SET(CPACK_CREATE_DESKTOP_LINKS "${PROJECT_NAME}")
include(CPack)
