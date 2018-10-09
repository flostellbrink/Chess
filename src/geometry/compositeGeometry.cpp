#include <GL/glew.h>

#include "compositeGeometry.hpp"
#include <iostream>

//TODO: merge child vertices into new lists

CompositeGeometry::CompositeGeometry(vector<Geometry*> geos) {
    _geos = geos;
}

CompositeGeometry::CompositeGeometry(Geometry* first, Geometry* second) {
    _geos = vector<Geometry*> { first, second };
}

void CompositeGeometry::Recreate() {
    for (Geometry* geo : _geos)
        geo->Recreate();
}

void CompositeGeometry::Draw() {
    for (Geometry* geo : _geos)
        geo->Draw();
}

void CompositeGeometry::Create() {

}


