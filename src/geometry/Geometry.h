// Loads geometry for all objects
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Geometry {
public:
  Geometry();
  virtual ~Geometry();
  virtual void Recreate();
  virtual void Draw();

protected:
  virtual void Create() = 0;

  GLuint vertex_array_object_;

  std::vector<glm::vec3> positions_;
  std::vector<glm::vec3> normals_;
  std::vector<unsigned> indices_;
  std::vector<glm::vec2> texture_coordinates_;

  void AddPosition(float x, float y, float z);
  void AddNormal(float x, float y, float z);
  void AddIndex(unsigned index);
  void AddTextureCoordinate(float x, float y);

private:
  void BeforeCreate();
  void FinishCreate();
};


#endif // GEOMETRY_H
