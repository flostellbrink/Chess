#include <GL/glew.h>
#include <algorithm>

#include "src/Logger.h"
#include "GeometryManager.h"
#include "src/Objects.h"
#include "CompositeGeometry.h"
#include "src/geometry/implementation/Square.h"
#include "src/geometry/implementation/Cube.h"
#include "src/geometry/implementation/Revolver.h"
#include "src/geometry/implementation/InterlacedRevolver.h"
#include "src/geometry/implementation/Extruder.h"
#include "src/geometry/implementation/ExtruderFace.h"
#include "src/geometry/implementation/BoardBorderTop.h"
#include "src/geometry/implementation/BoardBorderBottom.h"
#include "src/geometry/implementation/Quad.h"
#include "src/curve/FixedPointsCurve.h"
#include "src/curve/CircleCurve.h"
#include "src/curve/CatmullRomCurve.h"
#include "src/curve/LineCurve.h"

GeometryManager::GeometryManager() : theme_id_() {

}

void GeometryManager::Remove(Geometry *geo) {
  geos_.erase(std::remove(geos_.begin(), geos_.end(), geo), geos_.end());
}

void GeometryManager::SetTheme(int themeID) {
  theme_id_ = themeID;
  control_points_.SetTheme(themeID);
}

void GeometryManager::Regenerate() {
  for (Geometry *geo : geos_)
    geo->Recreate();
}

Geometry* GeometryManager::AddAndRecreate(Geometry *geo) {
  geo->Recreate();
  geos_.push_back(geo);
  return geo;
}

Geometry* GeometryManager::GetSquare() {
  return AddAndRecreate(new Square);
}

Geometry* GeometryManager::GetCube() {
  return AddAndRecreate(new Cube());
}

Geometry* GeometryManager::GetRevolver(int objectID) {
  return AddAndRecreate(new Revolver(new CircleCurve(), new CatmullRomCurve(control_points_, 0, objectID)));
}

Geometry* GeometryManager::GetRevolver(int objectID, float scale) {
  return AddAndRecreate(new Revolver(new CircleCurve(), new CatmullRomCurve(control_points_, 0, objectID), scale));
}

Geometry* GeometryManager::GetInterlacedRevolver(int objectID, float length1, float length2) {
  return AddAndRecreate(new InterlacedRevolver(new CircleCurve(), new CatmullRomCurve(control_points_, 0, objectID), new CatmullRomCurve(control_points_, 1, objectID), length1, length2));
}

Geometry* GeometryManager::GetRevolverAndExtruded(int objectID, float scale, float radius) {
  Curve *extruded = new CatmullRomCurve(control_points_, 1, objectID),
    *extrudedCompressed = new CatmullRomCurve(control_points_, 2, objectID),
    *extrudeAlong = new LineCurve(radius);
  return AddAndRecreate(new CompositeGeometry(std::vector<Geometry*> {
    GetRevolver(objectID),
      new Extruder(extruded, extrudeAlong, scale),
      new ExtruderFace(extruded, extrudedCompressed, extrudeAlong, 0, scale),
      new ExtruderFace(extruded, extrudedCompressed, extrudeAlong, 1, scale)
  }));
}

Geometry* GeometryManager::GetExtruded(int objectID, float scale, float radius) {
  Curve *extruded = new CatmullRomCurve(control_points_, 0, objectID),
    *extrudedCompressed = new CatmullRomCurve(control_points_, 1, objectID),
    *extrudeAlong = new LineCurve(radius);
  return AddAndRecreate(new CompositeGeometry(std::vector<Geometry*> {
    new Extruder(extruded, extrudeAlong, scale),
      new ExtruderFace(extruded, extrudedCompressed, extrudeAlong, 0, scale),
      new ExtruderFace(extruded, extrudedCompressed, extrudeAlong, 1, scale)
  }));
}

Geometry* GeometryManager::GetGeometry(int objectID) {
  switch (objectID) {
  case objects::white_pawn:
  case objects::black_pawn:
  case objects::white_bishop:
  case objects::black_bishop:
    return GetRevolver(objectID);
  case objects::white_rook:
  case objects::black_rook:
    return GetInterlacedRevolver(objectID, 0.2f, 0.05f);
  case objects::white_queen:
  case objects::black_queen:
    return GetInterlacedRevolver(objectID, 0.125f, 0.075f);
  case objects::white_knight:
  case objects::black_knight:
    return GetRevolverAndExtruded(objectID);
  case objects::table:
    return GetRevolver(objectID, 2);
  case objects::board_border_top:
    return AddAndRecreate(new BoardBorderTop());
  case objects::board_border_bottom:
    return AddAndRecreate(new BoardBorderBottom());
  case objects::clock_base:
    return GetExtruded(objectID, 0.5f, 0.1f);
  case objects::clock_leg:
  case objects::clock_button_border:
  case objects::clock_glass_border:
    return GetRevolver(objectID, 0.5f);
  case objects::clock_button:
    return GetRevolver(objectID, 0.13f);
  case objects::fullscreen_quad:
    return AddAndRecreate(new Quad());
  case objects::clock_face:
    return AddAndRecreate(new Revolver(new CircleCurve(), new CatmullRomCurve(control_points_, 0, objectID), 2.25f, true));
  case objects::white_minutes_hand:
  case objects::black_minutes_hand:
    return GetRevolver(objectID, 0.25f);
  case objects::white_seconds_hand:
  case objects::black_seconds_hand:
    return GetRevolver(objectID, 0.30f);
  case objects::clock_time_cap:
  case objects::clock_marks:
    return GetRevolver(objectID, 0.15f);
  default:break;
  }
  switch (theme_id_) {
  case themes::wood:
    switch (objectID) {
    case objects::white_king:
    case objects::black_king:
      return GetInterlacedRevolver(objectID, 0.075f, 0.05f);

    default:break;
    }
    break;
  case themes::glass:
    switch (objectID) {
    case objects::white_king:
    case objects::black_king:
      return GetRevolverAndExtruded(objectID);
    default:break;
    }
  default:break;
  }
  Logger::Error("ChessWarn: Using default geometry for objectID: " + std::to_string(objectID));
  return GetCube();
}
