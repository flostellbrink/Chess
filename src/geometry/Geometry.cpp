#include <GL/glew.h>

#include "Geometry.h"
#include <glm/glm.hpp>
#include "src/objects/ObjectManager.h"

Geometry::Geometry() : vertex_array_object_()
{
}

Geometry::~Geometry() {
  if (vertex_array_object_) {
    glDeleteVertexArrays(1, &vertex_array_object_);
  }
}

void Geometry::Recreate() {
  BeforeCreate();
  Create();
  FinishCreate();
}

void Geometry::Draw() {
  glBindVertexArray(vertex_array_object_);
  glDrawElements(GL_TRIANGLES, static_cast<int>(indices_.size()), GL_UNSIGNED_INT, nullptr);
  //glBindVertexArray(0);
}

void Geometry::BeforeCreate() {
  indices_.clear();
  positions_.clear();
  normals_.clear();
  texture_coordinates_.clear();
}

void Geometry::FinishCreate() {
  // Set up a vertex array object for the geometry
  if (!vertex_array_object_)
    glGenVertexArrays(1, &vertex_array_object_);
  glBindVertexArray(vertex_array_object_);

  // fill vertex array object with data
  GLuint position_buffer;
  glGenBuffers(1, &position_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
  glBufferData(GL_ARRAY_BUFFER, positions_.size() * 3 * sizeof(float), positions_.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(0);

  GLuint texture_buffer;
  glGenBuffers(1, &texture_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, texture_buffer);
  glBufferData(GL_ARRAY_BUFFER, texture_coordinates_.size() * sizeof(glm::vec2), texture_coordinates_.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(1);

  GLuint normal_buffer;
  glGenBuffers(1, &normal_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
  glBufferData(GL_ARRAY_BUFFER, normals_.size() * 3 * sizeof(float), normals_.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(2);

  GLuint index_buffer;
  glGenBuffers(1, &index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), GL_STATIC_DRAW);

  // unbind vertex array object
  glBindVertexArray(0);
  // delete buffers (the data is stored in the vertex array object)
  glDeleteBuffers(1, &position_buffer);
  glDeleteBuffers(1, &index_buffer);
  glDeleteBuffers(1, &normal_buffer);
  glDeleteBuffers(1, &texture_buffer);
}

void Geometry::AddPosition(float x, float y, float z) {
  positions_.emplace_back(x, y, z);
}

void Geometry::AddNormal(float x, float y, float z) {
  normals_.emplace_back(normalize(glm::vec3(x, y, z)));
}

void Geometry::AddIndex(unsigned index) {
  indices_.emplace_back(index);
}

void Geometry::AddTextureCoordinate(float x, float y) {
  texture_coordinates_.emplace_back(x, y);
}
