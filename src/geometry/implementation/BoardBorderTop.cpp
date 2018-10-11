#include "BoardBorderTop.h"


BoardBorderTop::BoardBorderTop()
= default;

void BoardBorderTop::Create() {
  // Positions
  AddPosition(-8, 0, -8);
  AddPosition(-8.5f, 0, -8.5f);
  AddPosition(8, 0, -8);
  AddPosition(8.5f, 0, -8.5f);

  // Normals
  AddNormal(0, 1, 0);
  AddNormal(0, 1, 0);
  AddNormal(0, 1, 0);
  AddNormal(0, 1, 0);

  // Default texture coordinates
  for (int i = 0; i < 4 * 1; ++i) {
    AddTextureCoordinate(0.0f, 0.0f);
  }

  // Indices
  AddIndex(0);
  AddIndex(1);
  AddIndex(2);
  AddIndex(1);
  AddIndex(2);
  AddIndex(3);
}
