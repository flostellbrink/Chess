#ifndef SQUARE_H
#define SQUARE_H

#include "src/geometry/Geometry.h"

class Square : public Geometry {
public:
  Square();

protected:
  void Create() override;
};


#endif // SQUARE_H
