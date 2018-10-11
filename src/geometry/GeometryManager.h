// Loads geometry for all objects
// Also handles recreation on reolution/ theme change
#ifndef GEOMANAGER_H
#define GEOMANAGER_H

#include <vector>
#include "ControlPoints.h"

class Geometry;

class GeometryManager {
public:
  GeometryManager();
  void SetTheme(int themeId);
  void Regenerate();
  void Remove(Geometry* geo);

  Geometry* GetSquare();
  Geometry* GetCube();
  Geometry* GetGeometry(int objectId);

protected:
  Geometry* AddAndRecreate(Geometry* geo);
  Geometry* GetRevolver(int objectId);
  Geometry* GetRevolver(int objectId, float scale);
  Geometry* GetInterlacedRevolver(int objectId, float length1, float length2);
  Geometry* GetRevolverAndExtruded(int objectId, float scale = .1f, float radius = .2f);
  Geometry* GetExtruded(int objectId, float scale = .1f, float radius = .2f);

  int theme_id_;
  std::vector<Geometry*> geos_;
  ControlPoints control_points_;
};


#endif // GEOMANAGER_H
