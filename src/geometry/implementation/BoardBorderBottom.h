#ifndef BOARDBORDERBOTTOM_H
#define BOARDBORDERBOTTOM_H

#include "src/geometry/Geometry.h"

class BoardBorderBottom : public Geometry {
public:
  BoardBorderBottom();

protected:
  void Create() override;
};


#endif // BOARDBORDERBOTTOM_H
