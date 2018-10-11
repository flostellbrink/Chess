#ifndef CUBE_H
#define CUBE_H

#include "src/geometry/Geometry.h"

class Cube : public Geometry {
public:
  Cube();

protected:
  void Create() override;
};


#endif // CUBE_H
