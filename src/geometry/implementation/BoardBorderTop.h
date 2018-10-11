#ifndef BOARDBORDERTOP_H
#define BOARDBORDERTOP_H

#include "src/geometry/Geometry.h"

class BoardBorderTop : public Geometry {
public:
  BoardBorderTop();

protected:
  void Create() override;
};


#endif // BOARDBORDERTOP_H
