#ifndef QUAD_H
#define QUAD_H

#include "src/geometry/Geometry.h"

class Quad : public Geometry {
public:
  Quad();

protected:
  void Create() override;
};


#endif // QUAD_H
