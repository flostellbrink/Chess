#include "Cube.h"


Cube::Cube()
= default;

void Cube::Create() {
  // Positions
  //front
  AddPosition(-1, +1, +1);
  AddPosition(+1, +1, +1);
  AddPosition(+1, -1, +1);
  AddPosition(-1, -1, +1);
  //back
  AddPosition(-1, +1, -1);
  AddPosition(+1, +1, -1);
  AddPosition(+1, -1, -1);
  AddPosition(-1, -1, -1);
  //right
  AddPosition(+1, +1, -1);
  AddPosition(+1, +1, +1);
  AddPosition(+1, -1, +1);
  AddPosition(+1, -1, -1);
  //left
  AddPosition(-1, +1, -1);
  AddPosition(-1, +1, +1);
  AddPosition(-1, -1, +1);
  AddPosition(-1, -1, -1);
  //top
  AddPosition(-1, +1, +1);
  AddPosition(+1, +1, +1);
  AddPosition(+1, +1, -1);
  AddPosition(-1, +1, -1);
  //bottom
  AddPosition(-1, -1, +1);
  AddPosition(+1, -1, +1);
  AddPosition(+1, -1, -1);
  AddPosition(-1, -1, -1);

  // Normals
  //front
  AddNormal(+0, +0, +1);
  AddNormal(+0, +0, +1);
  AddNormal(+0, +0, +1);
  AddNormal(+0, +0, +1);
  //back
  AddNormal(+0, +0, -1);
  AddNormal(+0, +0, -1);
  AddNormal(+0, +0, -1);
  AddNormal(+0, +0, -1);
  //right
  AddNormal(+1, +0, +0);
  AddNormal(+1, +0, +0);
  AddNormal(+1, +0, +0);
  AddNormal(+1, +0, +0);
  //left
  AddNormal(-1, +0, +0);
  AddNormal(-1, +0, +0);
  AddNormal(-1, +0, +0);
  AddNormal(-1, +0, +0);
  //top
  AddNormal(+0, +1, +0);
  AddNormal(+0, +1, +0);
  AddNormal(+0, +1, +0);
  AddNormal(+0, +1, +0);
  //bottom
  AddNormal(+0, -1, +0);
  AddNormal(+0, -1, +0);
  AddNormal(+0, -1, +0);
  AddNormal(+0, -1, +0);

  // Texture Coordinates
  //front
  AddTextureCoordinate(0, 1);
  AddTextureCoordinate(1, 1);
  AddTextureCoordinate(1, 0);
  AddTextureCoordinate(0, 0);
  //back
  AddTextureCoordinate(1, 1);
  AddTextureCoordinate(0, 1);
  AddTextureCoordinate(0, 0);
  AddTextureCoordinate(1, 0);
  //right
  AddTextureCoordinate(1, 1);
  AddTextureCoordinate(0, 1);
  AddTextureCoordinate(0, 0);
  AddTextureCoordinate(1, 0);
  //left
  AddTextureCoordinate(0, 1);
  AddTextureCoordinate(1, 1);
  AddTextureCoordinate(1, 0);
  AddTextureCoordinate(0, 0);
  //top
  AddTextureCoordinate(0, 0);
  AddTextureCoordinate(1, 0);
  AddTextureCoordinate(1, 1);
  AddTextureCoordinate(0, 1);
  //bottom
  AddTextureCoordinate(0, 1);
  AddTextureCoordinate(1, 1);
  AddTextureCoordinate(1, 0);
  AddTextureCoordinate(0, 0);

  // Indices
  //front
  AddIndex(0);
  AddIndex(1);
  AddIndex(3);
  AddIndex(1);
  AddIndex(2);
  AddIndex(3);
  //back
  AddIndex(4);
  AddIndex(5);
  AddIndex(7);
  AddIndex(5);
  AddIndex(6);
  AddIndex(7);
  //right
  AddIndex(8);
  AddIndex(9);
  AddIndex(11);
  AddIndex(9);
  AddIndex(10);
  AddIndex(11);
  //left
  AddIndex(12);
  AddIndex(13);
  AddIndex(15);
  AddIndex(13);
  AddIndex(14);
  AddIndex(15);
  //top
  AddIndex(16);
  AddIndex(17);
  AddIndex(19);
  AddIndex(17);
  AddIndex(18);
  AddIndex(19);
  //bottom
  AddIndex(20);
  AddIndex(21);
  AddIndex(23);
  AddIndex(21);
  AddIndex(22);
  AddIndex(23);
}
