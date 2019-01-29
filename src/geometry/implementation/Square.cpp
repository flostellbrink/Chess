#include "Square.h"

Square::Square()
= default;

void Square::Create()
{
  AddPosition(-1, 0, -1);
  AddPosition(1, 0, -1);
  AddPosition(-1, 0, 1);
  AddPosition(1, 0, 1);

  AddIndex(1);
  AddIndex(2);
  AddIndex(3);

  AddIndex(0);
  AddIndex(2);
  AddIndex(1);
}
