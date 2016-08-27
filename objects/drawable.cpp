#include <GL/glew.h>

#include "drawable.hpp"

#include <iostream>

#include <QFile>
#include <QTextStream>

#include "glbase/gltool.hpp"

#include "texture/image.hpp"
#include "objects/objectManager.hpp"

vec3 Drawable::_lightPos = vec3(0,20,0);
vec3 Drawable::_camPos = vec3();
GLuint Drawable::ReflectionTexture;
GLuint Drawable::PostTexture;
GLuint Drawable::ShadowTexture;
mat4 Drawable::ShadowViewProjection;
int Drawable::OverlayState = 0;
float Drawable::OverlayOpacity = 1.f;

Drawable::Drawable(int objectId):
    _vertexArrayObject(0), _modelViewMatrix(1.0f), _objectId(objectId) {
}

void Drawable::init()
{
    initShader();
}

void Drawable::initShader()
{
    GLuint vs = CG::createCompileShader(GL_VERTEX_SHADER, getVertexShader()); VERIFY(vs);
    GLuint fs = CG::createCompileShader(GL_FRAGMENT_SHADER, getFragmentShader()); VERIFY(fs);
    _program = glCreateProgram();
    glAttachShader(_program, vs);
    glAttachShader(_program, fs);
    _program = CG::linkProgram(_program);
    VERIFY(_program);

    vs = CG::createCompileShader(GL_VERTEX_SHADER, loadShaderFile(":/shader/shadow.vs.glsl")); VERIFY(vs);
    fs = CG::createCompileShader(GL_FRAGMENT_SHADER, loadShaderFile(":/shader/shadow.fs.glsl")); VERIFY(fs);
    _programShadow = glCreateProgram();
    glAttachShader(_programShadow, vs);
    glAttachShader(_programShadow, fs);
    _programShadow = CG::linkProgram(_programShadow);
    VERIFY(_programShadow);
}

void Drawable::drawOpaque(glm::mat4 projection_matrix){
    if(!ObjectManager::Textures.IsTranslucent(_objectId))
        draw(projection_matrix);
}

void Drawable::drawTranslucent(glm::mat4 projection_matrix){
    if(ObjectManager::Textures.IsTranslucent(_objectId))
        draw(projection_matrix);
}

void Drawable::drawReflection(glm::mat4 projection_matrix){
    // Ignores if translucent or not, because only translucent or only opaque objects are drawn
    if(ObjectManager::Textures.IsReflected(_objectId))
        draw(projection_matrix);
}

std::string Drawable::loadShaderFile(std::string path)
{
    QFile f(path.c_str());
    if (!f.open(QFile::ReadOnly | QFile::Text))
        std::cout << "Could not open file " << path << std::endl;
    QTextStream in(&f);
    return in.readAll().toStdString();
}
