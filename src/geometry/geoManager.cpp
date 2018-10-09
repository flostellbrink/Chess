#include <GL/glew.h>
#include <algorithm>
#include <iostream>

#include "geoManager.hpp"
#include "src/objects.hpp"

#include "compositeGeometry.hpp"

#include "implementation/square.hpp"
#include "implementation/cube.hpp"
#include "implementation/revolver.hpp"
#include "implementation/interlacedRevolver.hpp"
#include "implementation/extruder.hpp"
#include "implementation/extruderFace.hpp"
#include "implementation/boardBorderTop.hpp"
#include "implementation/boardBorderBottom.hpp"
#include "implementation/quad.hpp"

#include "src/curve/fixedPointsCurve.hpp"
#include "src/curve/circleCurve.hpp"
#include "src/curve/catmullRomCurve.hpp"
#include "src/curve/lineCurve.hpp"

//TODO implement geometry recycling

GeoManager::GeoManager()
= default;

void GeoManager::Remove(Geometry *geo){
    _geos.erase(std::remove(_geos.begin(), _geos.end(), geo), _geos.end());
}

void GeoManager::SetTheme(int themeID){
   _themeID = themeID;
   _controlPoints.SetTheme(themeID);
}

void GeoManager::Regenerate(){
    for(Geometry *geo : _geos)
        geo->Recreate();
}

Geometry* GeoManager::AddAndRecreate(Geometry *geo){
    geo->Recreate();
    _geos.push_back(geo);
    return geo;
}

Geometry* GeoManager::GetSquare(){
    return AddAndRecreate(new Square);
}

Geometry* GeoManager::GetCube(){
    return AddAndRecreate(new Cube());
}

Geometry* GeoManager::GetRevolver(int objectID){
    return AddAndRecreate(new Revolver(new CircleCurve(), new CatmullRomCurve(_controlPoints, 0, objectID)));
}

Geometry* GeoManager::GetRevolver(int objectID, float scale){
    return AddAndRecreate(new Revolver(new CircleCurve(), new CatmullRomCurve(_controlPoints, 0, objectID), scale));
}

Geometry* GeoManager::GetInterlacedRevolver(int objectID, float length1, float length2){
    return AddAndRecreate(new InterlacedRevolver(new CircleCurve(), new CatmullRomCurve(_controlPoints, 0, objectID), new CatmullRomCurve(_controlPoints, 1, objectID), length1, length2));
}

Geometry* GeoManager::GetRevolverAndExtruded(int objectID, float scale, float radius){
    Curve *extruded = new CatmullRomCurve(_controlPoints, 1, objectID),
            *extrudedCompressed = new CatmullRomCurve(_controlPoints, 2, objectID),
            *extrudeAlong = new LineCurve(radius);
    return AddAndRecreate(new CompositeGeometry(vector<Geometry*> {
                GetRevolver(objectID),
                new Extruder(extruded, extrudeAlong, scale),
                new ExtruderFace(extruded, extrudedCompressed, extrudeAlong, 0, scale),
                new ExtruderFace(extruded, extrudedCompressed, extrudeAlong, 1, scale)
    }));
}

Geometry* GeoManager::GetExtruded(int objectID, float scale, float radius){
    Curve *extruded = new CatmullRomCurve(_controlPoints, 0, objectID),
            *extrudedCompressed = new CatmullRomCurve(_controlPoints, 1, objectID),
            *extrudeAlong = new LineCurve(radius);
    return AddAndRecreate(new CompositeGeometry(vector<Geometry*> {
                new Extruder(extruded, extrudeAlong, scale),
                new ExtruderFace(extruded, extrudedCompressed, extrudeAlong, 0, scale),
                new ExtruderFace(extruded, extrudedCompressed, extrudeAlong, 1, scale)
    }));
}

Geometry* GeoManager::GetGeometry(int objectID){
    switch( objectID ) {
        case objects::whitePawn:
        case objects::blackPawn:
        case objects::whiteBishop:
        case objects::blackBishop:
            return GetRevolver(objectID);
        case objects::whiteRook:
        case objects::blackRook:
            return GetInterlacedRevolver(objectID, .2, .05);
        case objects::whiteQueen:
        case objects::blackQueen:
            return GetInterlacedRevolver(objectID, .125, .075);
        case objects::whiteKnight:
        case objects::blackKnight:
            return GetRevolverAndExtruded(objectID);
        case objects::Table:
            return GetRevolver(objectID, 2);
        case objects::BoardBorderTop:
            return AddAndRecreate(new BoardBorderTop());
        case objects::BoardBorderBottom:
            return AddAndRecreate(new BoardBorderBottom());
        case objects::ClockBase:
            return GetExtruded(objectID, .5, .1);
        case objects::ClockLeg:
        case objects::ClockButtonBorder:
        case objects::ClockGlassBorder:
            return GetRevolver(objectID, .5);
        case objects::ClockButton:
            return GetRevolver(objectID, .13);
        case objects::FullscreenQuad:
            return AddAndRecreate(new Quad());
        case objects::ClockFace:
            return AddAndRecreate(new Revolver(new CircleCurve(), new CatmullRomCurve(_controlPoints, 0, objectID), 2.25, true));
        case objects::whiteMinutesHand:
        case objects::blackMinutesHand:
           return GetRevolver(objectID, .25);
        case objects::whiteSecondsHand:
        case objects::blackSecondsHand:
            return GetRevolver(objectID, .30);
        case objects::ClockTimeCap:
        case objects::ClockMarks:
            return GetRevolver(objectID, .15);
        default:break;
    }
    switch( _themeID ) {
        case themes::Default:
            switch( objectID ) {
                case objects::whiteKing:
                case objects::blackKing:
                    return GetInterlacedRevolver(objectID, .075, .05);

                default:break;
            }
        break;
        case themes::Glass:
            switch( objectID ){
                case objects::whiteKing:
                case objects::blackKing:
                    return GetRevolverAndExtruded(objectID);
                default:break;
            }
        default:break;
    }
    std::cerr << "ChessWarn: Using default geometry for objectID: " << objectID << std::endl;
    return GetCube();
}
