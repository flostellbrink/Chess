// Contains an enum for every object, used for textures and geometry

#ifndef OBJECTS_H
#define OBJECTS_H

#include <GL/glew.h>

namespace objects
{
    const int
    Board = 0,
    Skybox = 1,
    Field00 = 2,
    Field10 = 3,
    // Field 11 to Field 67 implicit
    Field77 = 65,
    whiteKing = 66,
    blackKing = 67,
    whiteQueen = 68,
    blackQueen = 69,
    whiteRook = 70,
    blackRook = 71,
    whiteBishop = 72,
    blackBishop = 73,
    whiteKnight = 74,
    blackKnight = 75,
    whitePawn = 76,
    blackPawn = 77,
    Table = 78,
    BoardBorderTop = 79,
    BoardBorderBottom = 80,
    ClockBase = 81,
    ClockLeg = 82,
    ClockGlass = 83,
    ClockButton = 84,
    ClockPointers = 85,
    ClockGlassBorder = 86,
    ClockButtonBorder = 87,
    FullscreenQuad = 88,
    ClockFace = 89,
    whiteMinutesHand = 90,
    whiteSecondsHand = 91,
    blackMinutesHand = 92,
    blackSecondsHand = 93,
    ClockTimeCap = 94,
    ClockMarks = 95;
}

namespace themes {
    const unsigned int
    Default = 0,
    Metal = 1,
    Glass = 2;
}

#endif // OBJECTS_H
