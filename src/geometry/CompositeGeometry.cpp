#include <utility>

#include "CompositeGeometry.h"

//TODO: merge child vertices into new lists

CompositeGeometry::CompositeGeometry(std::vector<Geometry*> geos) {
  geos_ = std::move(geos);
}

CompositeGeometry::CompositeGeometry(Geometry* first, Geometry* second) {
  geos_ = std::vector<Geometry*>{ first, second };
}

void CompositeGeometry::Recreate() {
  for (Geometry* geo : geos_)
    geo->Recreate();
}

void CompositeGeometry::Draw() {
  for (Geometry* geo : geos_)
    geo->Draw();
}

void CompositeGeometry::Create() {

}


