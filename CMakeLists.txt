# CMake entry point
cmake_minimum_required(VERSION 3.5)
set (CMAKE_CXX_STANDARD 14)
set(PROJECT_NAME "Chess")
project(${PROJECT_NAME})

include(conanbuildinfo.cmake)
conan_basic_setup()

find_package(OpenGL REQUIRED)

include_directories(.)

add_executable(${PROJECT_NAME}
        src/Main.cpp
        src/Objects.h
        src/Config.h src/Config.cpp
        src/Logger.h src/Logger.cpp
        src/Window.h src/Window.cpp
        src/objects/Camera.h src/objects/Camera.cpp
        src/objects/ObjectManager.h src/objects/ObjectManager.cpp
        src/objects/Drawable.h src/objects/Drawable.cpp
        src/objects/BasicObject.h src/objects/BasicObject.cpp
        src/objects/FullScreenQuad.h src/objects/FullScreenQuad.cpp
        src/objects/Board.h src/objects/Board.cpp
        src/objects/Field.h src/objects/Field.cpp
        src/objects/Piece.h src/objects/Piece.cpp
        src/objects/pieces/Bishop.h src/objects/pieces/Bishop.cpp
        src/objects/pieces/King.h src/objects/pieces/King.cpp
        src/objects/pieces/Knight.h src/objects/pieces/Knight.cpp
        src/objects/pieces/Pawn.h src/objects/pieces/Pawn.cpp
        src/objects/pieces/Queen.h src/objects/pieces/Queen.cpp
        src/objects/pieces/Rook.h src/objects/pieces/Rook.cpp
        src/objects/Clock.h src/objects/Clock.cpp
        src/objects/Skybox.h src/objects/Skybox.cpp
        src/objects/moves/MoveBase.h src/objects/moves/MoveBase.cpp
        src/objects/moves/Move.h src/objects/moves/Move.cpp
        src/objects/moves/Hit.h src/objects/moves/Hit.cpp
        src/objects/moves/Castling.h src/objects/moves/Castling.cpp
        src/objects/moves/Transform.h src/objects/moves/Transform.cpp
        src/collision/CollisionManager.h src/collision/CollisionManager.cpp
        src/collision/Collision.h src/collision/Collision.cpp
        src/geometry/GeometryManager.h src/geometry/GeometryManager.cpp
        src/geometry/ControlPoints.h src/geometry/ControlPoints.cpp
        src/geometry/Geometry.h src/geometry/Geometry.cpp
        src/geometry/CompositeGeometry.h src/geometry/CompositeGeometry.cpp
        src/geometry/implementation/BoardBorderBottom.h src/geometry/implementation/BoardBorderBottom.cpp
        src/geometry/implementation/BoardBorderTop.h src/geometry/implementation/BoardBorderTop.cpp
        src/geometry/implementation/Cube.h src/geometry/implementation/Cube.cpp
        src/geometry/implementation/Extruder.h src/geometry/implementation/Extruder.cpp
        src/geometry/implementation/ExtruderFace.h src/geometry/implementation/ExtruderFace.cpp
        src/geometry/implementation/InterlacedRevolver.h src/geometry/implementation/InterlacedRevolver.cpp
        src/geometry/implementation/Quad.h src/geometry/implementation/Quad.cpp
        src/geometry/implementation/Revolver.h src/geometry/implementation/Revolver.cpp
        src/geometry/implementation/Square.h src/geometry/implementation/Square.cpp
        src/curve/Curve.h src/curve/Curve.cpp
        src/curve/CatmullRomCurve.h src/curve/CatmullRomCurve.cpp
        src/curve/CircleCurve.h src/curve/CircleCurve.cpp
        src/curve/FixedPointsCurve.h src/curve/FixedPointsCurve.cpp
        src/curve/LineCurve.h src/curve/LineCurve.cpp
        src/animation/AnimationManager.h src/animation/AnimationManager.cpp
        src/animation/AnimationBase.h
        src/animation/Animation.h
        src/animation/LoopingAnimation.h
        src/animation/BackLoopingAnimation.h
        src/animation/CatmullRomAnimation.h
        src/animation/FadeAnimation.h
        src/animation/LinearAnimation.h
        src/animation/DelayAnimation.h
        src/animation/GroupAnimation.h src/animation/GroupAnimation.cpp
        src/texture/TextureManager.h src/texture/TextureManager.cpp
        src/texture/Image.h src/texture/Image.cpp
        src/texture/Texture.h src/texture/Texture.cpp
        src/texture/Shader.h src/texture/Shader.cpp
        )

# Enable emscripten performance optimizations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -s USE_LIBPNG=1")

target_link_libraries(${PROJECT_NAME}
        ${CONAN_LIBS}
        ${OPENGL_LIBRARY}
        # Emscripten options
        "-O2"
        "-s USE_WEBGL2=1"
        "-s FULL_ES3=1"
        "-s USE_GLFW=3"
        "-s WASM=1"
        "-s DEMANGLE_SUPPORT=1"
        "-s ALLOW_MEMORY_GROWTH=1"
        "-o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/index.html"
        "--preload-file res"
        "--bind"
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
