// Loads geometry for all objects
// Also handles recreation on resolution/ theme change
#ifndef GEOMETRY_MANAGER_H
#define GEOMETRY_MANAGER_H

#include <unordered_map>
#include "ControlPoints.h"

class Geometry;

class GeometryManager {
public:
  GeometryManager();
  void SetTheme(int themeId);
  void Regenerate();
  Geometry* GetGeometryCached(int objectId);

protected:
  Geometry* GetGeometry(int objectId);
  Geometry* GetRevolver(int objectId);
  Geometry* GetRevolver(int objectId, float scale);
  Geometry* GetInterlacedRevolver(int objectId, float length1, float length2);
  Geometry* GetRevolverAndExtruded(int objectId, float scale = .1f, float radius = .2f);
  Geometry* GetExtruded(int objectId, float scale = .1f, float radius = .2f);

  int theme_id_;
  std::unordered_map<int, Geometry*> geometries_;
  ControlPoints control_points_;
};


#endif // GEOMETRY_MANAGER_H
