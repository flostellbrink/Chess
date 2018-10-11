#include "Quad.h"


Quad::Quad()
= default;

void Quad::Create() {
  // Positions
  AddPosition(+1, +1, +0);
  AddPosition(+1, -1, +0);
  AddPosition(-1, -1, +0);
  AddPosition(-1, +1, +0);

  // Normals
  AddNormal(+0, +0, +1);
  AddNormal(+0, +0, +1);
  AddNormal(+0, +0, +1);
  AddNormal(+0, +0, +1);

  // Texture Coordinates
  AddTextureCoordinate(1, 1);
  AddTextureCoordinate(1, 0);
  AddTextureCoordinate(0, 0);
  AddTextureCoordinate(0, 1);

  // Indices
  AddIndex(0);
  AddIndex(1);
  AddIndex(3);
  AddIndex(1);
  AddIndex(2);
  AddIndex(3);
}
