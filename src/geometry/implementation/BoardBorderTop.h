#ifndef BOARD_BORDER_TOP_H
#define BOARD_BORDER_TOP_H

#include "src/geometry/Geometry.h"

class BoardBorderTop : public Geometry
{
public:
  BoardBorderTop();

protected:
  void Create() override;
};


#endif // BOARD_BORDER_TOP_H
