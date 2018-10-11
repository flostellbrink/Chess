#ifndef COMPOSITEGEOMETRY_H
#define COMPOSITEGEOMETRY_H

#include "src/geometry/Geometry.h"

class CompositeGeometry : public Geometry {
public:
  explicit CompositeGeometry(std::vector<Geometry*> geos);
  CompositeGeometry(Geometry* first, Geometry* second);

  void Recreate() override;
  void Draw() override;

protected:
  void Create() override;

private:
  std::vector<Geometry*> geos_;
};


#endif // COMPOSITEGEOMETRY_H
