#include <utility>

#include "CompositeGeometry.h"

CompositeGeometry::CompositeGeometry(std::vector<Geometry*> geometries) {
  geometries_ = std::move(geometries);
}

CompositeGeometry::CompositeGeometry(Geometry* first, Geometry* second) {
  geometries_ = std::vector<Geometry*>{ first, second };
}

void CompositeGeometry::Recreate() {
  for (Geometry* geo : geometries_)
    geo->Recreate();
}

void CompositeGeometry::Draw() {
  for (Geometry* geo : geometries_)
    geo->Draw();
}

void CompositeGeometry::Create() {

}


