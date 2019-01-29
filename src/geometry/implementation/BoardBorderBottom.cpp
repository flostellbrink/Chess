#include "BoardBorderBottom.h"


BoardBorderBottom::BoardBorderBottom()
= default;

void BoardBorderBottom::Create()
{
  // Positions
  AddPosition(-8.5f, 0, -8.5f);
  AddPosition(-8.5f, -0.1f, -8.5f);
  AddPosition(8.5f, 0, -8.5f);
  AddPosition(8.5f, -0.1f, -8.5f);

  AddPosition(-8.5f, -0.1f, -8.5f);
  AddPosition(-10, -0.8f, -10);
  AddPosition(8.5f, -0.1f, -8.5f);
  AddPosition(10, -0.8f, -10);

  AddPosition(-10, -0.8f, -10);
  AddPosition(-10, -1, -10);
  AddPosition(10, -0.8f, -10);
  AddPosition(10, -1, -10);

  // Normals
  AddNormal(0, 0, -1);
  AddNormal(0, 0, -1);
  AddNormal(0, 0, -1);
  AddNormal(0, 0, -1);

  AddNormal(0, 1, -1);
  AddNormal(0, 1, -1);
  AddNormal(0, 1, -1);
  AddNormal(0, 1, -1);

  AddNormal(0, 0, -1);
  AddNormal(0, 0, -1);
  AddNormal(0, 0, -1);
  AddNormal(0, 0, -1);

  // Texture Coordinates, If these are not defined windows 7 breaks down.
  for (auto i = 0; i < 4 * 3; ++i)
  {
    AddTextureCoordinate(0.0f, 0.0f);
  }

  // Texture Coordinates not needed, mapped from world position

  // Indices
  AddIndex(0);
  AddIndex(1);
  AddIndex(2);
  AddIndex(1);
  AddIndex(2);
  AddIndex(3);

  AddIndex(4);
  AddIndex(5);
  AddIndex(6);
  AddIndex(5);
  AddIndex(6);
  AddIndex(7);

  AddIndex(8);
  AddIndex(9);
  AddIndex(10);
  AddIndex(9);
  AddIndex(10);
  AddIndex(11);
}
