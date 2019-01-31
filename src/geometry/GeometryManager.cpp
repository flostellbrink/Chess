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

GeometryManager::GeometryManager() : theme_id_()
{
}

void GeometryManager::SetTheme(const int themeId)
{
  theme_id_ = themeId;
  control_points_.SetTheme(themeId);
  Regenerate();
}

void GeometryManager::Regenerate()
{
  for (auto& pair : geometries_)
  {
    pair.second->Recreate();
  }
}

Geometry* GeometryManager::GetGeometryCached(int objectId)
{
  // Use a single identifier for all fields
  if (objectId >= objects::field00 && objectId <= objects::field77)
  {
    objectId = objects::field00;
  }

  const auto iterator = geometries_.find(objectId);
  if (iterator != geometries_.end())
  {
    return iterator->second;
  }

  auto geo = GetGeometry(objectId);
  geo->Recreate();
  geometries_[objectId] = geo;
  return geo;
}

Geometry* GeometryManager::GetRevolver(const int objectId) const
{
  return new Revolver(new CircleCurve(), new CatmullRomCurve(control_points_, 0, objectId));
}

Geometry* GeometryManager::GetRevolver(const int objectId, const float scale) const
{
  return new Revolver(new CircleCurve(), new CatmullRomCurve(control_points_, 0, objectId), scale);
}

Geometry* GeometryManager::GetInterlacedRevolver(const int objectId, const float length1, const float length2) const
{
  return new InterlacedRevolver(new CircleCurve(),
                                new CatmullRomCurve(control_points_, 0, objectId),
                                new CatmullRomCurve(control_points_, 1, objectId),
                                length1,
                                length2);
}

Geometry* GeometryManager::GetRevolverAndExtruded(const int objectId, const float scale, const float radius) const
{
  Curve *extruded = new CatmullRomCurve(control_points_, 1, objectId),
        *extrudedCompressed = new CatmullRomCurve(control_points_, 2, objectId),
        *extrudeAlong = new LineCurve(radius);
  return new CompositeGeometry(std::vector<Geometry*>{
    GetRevolver(objectId),
    new Extruder(extruded, extrudeAlong, scale),
    new ExtruderFace(extruded, extrudedCompressed, extrudeAlong, 0, scale),
    new ExtruderFace(extruded, extrudedCompressed, extrudeAlong, 1, scale)
  });
}

Geometry* GeometryManager::GetExtruded(const int objectId, const float scale, const float radius) const
{
  Curve *extruded = new CatmullRomCurve(control_points_, 0, objectId),
        *extrudedCompressed = new CatmullRomCurve(control_points_, 1, objectId),
        *extrudeAlong = new LineCurve(radius);
  return new CompositeGeometry(std::vector<Geometry*>{
    new Extruder(extruded, extrudeAlong, scale),
    new ExtruderFace(extruded, extrudedCompressed, extrudeAlong, 0, scale),
    new ExtruderFace(extruded, extrudedCompressed, extrudeAlong, 1, scale)
  });
}

Geometry* GeometryManager::GetGeometry(const int objectId) const
{
  switch (objectId)
  {
  case objects::skybox:
  case objects::field00:
    return new Cube;
  case objects::white_pawn:
  case objects::black_pawn:
  case objects::white_bishop:
  case objects::black_bishop:
    return GetRevolver(objectId);
  case objects::white_rook:
  case objects::black_rook:
    return GetInterlacedRevolver(objectId, 0.2f, 0.05f);
  case objects::white_queen:
  case objects::black_queen:
    return GetInterlacedRevolver(objectId, 0.125f, 0.075f);
  case objects::white_knight:
  case objects::black_knight:
    return GetRevolverAndExtruded(objectId);
  case objects::table:
    return GetRevolver(objectId, 2);
  case objects::board_border_top:
    return new BoardBorderTop();
  case objects::board_border_bottom:
    return new BoardBorderBottom();
  case objects::clock_base:
    return GetExtruded(objectId, 0.5f, 0.1f);
  case objects::clock_leg:
  case objects::clock_button_border:
  case objects::clock_glass_border:
    return GetRevolver(objectId, 0.5f);
  case objects::clock_button:
    return GetRevolver(objectId, 0.13f);
  case objects::fullscreen_quad:
    return new Quad;
  case objects::clock_face:
    return new Revolver(new CircleCurve(), new CatmullRomCurve(control_points_, 0, objectId), 2.25f, true);
  case objects::white_minutes_hand:
  case objects::black_minutes_hand:
    return GetRevolver(objectId, 0.25f);
  case objects::white_seconds_hand:
  case objects::black_seconds_hand:
    return GetRevolver(objectId, 0.30f);
  case objects::clock_time_cap:
  case objects::clock_marks:
    return GetRevolver(objectId, 0.15f);
  default: break;
  }
  switch (theme_id_)
  {
  case themes::wood:
    switch (objectId)
    {
    case objects::white_king:
    case objects::black_king:
      return GetInterlacedRevolver(objectId, 0.075f, 0.05f);

    default: break;
    }
    break;
  case themes::glass:
    switch (objectId)
    {
    case objects::white_king:
    case objects::black_king:
      return GetRevolverAndExtruded(objectId);
    default: break;
    }
  default: break;
  }
  Logger::Error("Using default geometry for objectID: " + std::to_string(objectId));
  return new Cube;
}
