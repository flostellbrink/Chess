#ifndef BOARD_BORDER_BOTTOM_H
#define BOARD_BORDER_BOTTOM_H

#include "src/geometry/Geometry.h"

class BoardBorderBottom : public Geometry
{
public:
  BoardBorderBottom();

protected:
  void Create() override;
};


#endif // BOARD_BORDER_BOTTOM_H
