#include <GL/glew.h>

#include "Geometry.h"
#include <glm/glm.hpp>
#include "src/objects/ObjectManager.h"

Geometry::Geometry() : vertex_array_object_()
{
}

Geometry::~Geometry()
{
  if (vertex_array_object_)
  {
    glDeleteVertexArrays(1, &vertex_array_object_);
  }
}

void Geometry::Recreate()
{
  BeforeCreate();
  Create();
  FinishCreate();
}

void Geometry::Draw()
{
  glBindVertexArray(vertex_array_object_);
  glDrawElements(GL_TRIANGLES, static_cast<int>(indices_.size()), GL_UNSIGNED_INT, nullptr);
  //glBindVertexArray(0);
}

void Geometry::BeforeCreate()
{
  indices_.clear();
  positions_.clear();
  normals_.clear();
  texture_coordinates_.clear();
}

void Geometry::FinishCreate()
{
  // Set up a vertex array object for the geometry
  if (!vertex_array_object_)
    glGenVertexArrays(1, &vertex_array_object_);
  glBindVertexArray(vertex_array_object_);

  // fill vertex array object with data
  GLuint positionBuffer;
  glGenBuffers(1, &positionBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
  glBufferData(GL_ARRAY_BUFFER, positions_.size() * 3 * sizeof(float), positions_.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(0);

  GLuint textureBuffer;
  glGenBuffers(1, &textureBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
  glBufferData(GL_ARRAY_BUFFER,
               texture_coordinates_.size() * sizeof(glm::vec2),
               texture_coordinates_.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(1);

  GLuint normalBuffer;
  glGenBuffers(1, &normalBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
  glBufferData(GL_ARRAY_BUFFER, normals_.size() * 3 * sizeof(float), normals_.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(2);

  GLuint indexBuffer;
  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), GL_STATIC_DRAW);

  // unbind vertex array object
  glBindVertexArray(0);
  // delete buffers (the data is stored in the vertex array object)
  glDeleteBuffers(1, &positionBuffer);
  glDeleteBuffers(1, &indexBuffer);
  glDeleteBuffers(1, &normalBuffer);
  glDeleteBuffers(1, &textureBuffer);
}

void Geometry::AddPosition(float x, float y, float z)
{
  positions_.emplace_back(x, y, z);
}

void Geometry::AddNormal(const float x, const float y, const float z)
{
  normals_.emplace_back(normalize(glm::vec3(x, y, z)));
}

void Geometry::AddIndex(unsigned index)
{
  assert(positions_.size() > index);
  assert(normals_.size() > index);
  indices_.emplace_back(index);
}

void Geometry::AddTextureCoordinate(float x, float y)
{
  texture_coordinates_.emplace_back(x, y);
}
