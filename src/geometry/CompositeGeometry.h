#ifndef COMPOSITE_GEOMETRY_H
#define COMPOSITE_GEOMETRY_H

#include "src/geometry/Geometry.h"

class CompositeGeometry : public Geometry
{
public:
  explicit CompositeGeometry(std::vector<Geometry*> geometries);
  CompositeGeometry(Geometry* first, Geometry* second);

  void Recreate() override;
  void Draw() override;

protected:
  void Create() override;

private:
  std::vector<Geometry*> geometries_;
};


#endif // COMPOSITE_GEOMETRY_H
