// Loads geometry for all objects
// Also handles recreation on reolution/ theme change
#ifndef GEOMANAGER_H
#define GEOMANAGER_H

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
  std::unordered_map<int, Geometry*> geos_;
  ControlPoints control_points_;
};


#endif // GEOMANAGER_H
