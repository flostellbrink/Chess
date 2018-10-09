#include <GL/glew.h>

#include "geometry.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "src/objects/objectManager.hpp"

Geometry::Geometry() : _vertexArrayObject()
{
}

Geometry::~Geometry(){
    if(_vertexArrayObject)
        glDeleteVertexArrays(1, &_vertexArrayObject);
    ObjectManager::Instance.Geos.Remove(this);
}

void Geometry::Recreate(){
    BeforeCreate();
    Create();
    FinishCreate();
}

void Geometry::Draw(){
    glBindVertexArray(_vertexArrayObject);
    glDrawElements(GL_TRIANGLES, _geoIndices.size(), GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
}

void Geometry::BeforeCreate(){
    _geoIndices.clear();
    _geoPositions.clear();
    _geoNormals.clear();
    _geoTextureCoords.clear();
}

void Geometry::FinishCreate(){
    // Set up a vertex array object for the geometry
    if(!_vertexArrayObject)
        glGenVertexArrays(1, &_vertexArrayObject);
    glBindVertexArray(_vertexArrayObject);

    // fill vertex array object with data
    GLuint position_buffer;
    glGenBuffers(1, &position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    glBufferData(GL_ARRAY_BUFFER, _geoPositions.size() * 3 * sizeof(float), _geoPositions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    GLuint texture_buffer;
    glGenBuffers(1, &texture_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, texture_buffer);
    glBufferData(GL_ARRAY_BUFFER, _geoTextureCoords.size() * sizeof(glm::vec2), _geoTextureCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    GLuint normal_buffer;
    glGenBuffers(1, &normal_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glBufferData(GL_ARRAY_BUFFER, _geoNormals.size() * 3 * sizeof(float), _geoNormals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    GLuint index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _geoIndices.size() * sizeof(unsigned int), _geoIndices.data(), GL_STATIC_DRAW);

    // unbind vertex array object
    glBindVertexArray(0);
    // delete buffers (the data is stored in the vertex array object)
    glDeleteBuffers(1, &position_buffer);
    glDeleteBuffers(1, &index_buffer);
    glDeleteBuffers(1, &normal_buffer);
    glDeleteBuffers(1, &texture_buffer);
}
