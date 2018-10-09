#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "skybox.hpp"
#include "objectManager.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <src/objects.hpp>

#include "vector"



Skybox::Skybox(): Drawable(objects::Skybox) {
	
}

void Skybox::init(){
    Drawable::init();
	_geo = ObjectManager::Geos.GetCube();
}

void Skybox::draw(glm::mat4 projection_matrix){
    // Ignore parameter unused
    (void)projection_matrix;
}

void Skybox::drawShadow(glm::mat4 projection_matrix){
    // Ignore parameter unused
    (void)projection_matrix;
}

void Skybox::drawSkybox(glm::mat4 projection_matrix){
    // Load program
    _program->use();

    ObjectManager::Textures.Texture(_objectId)[0]->Bind(GL_TEXTURE_CUBE_MAP);

    // set parameter
    _program->bind(projection_matrix, "projection_matrix");
    _program->bind(_modelViewMatrix, "modelview_matrix");

    glDisable(GL_DEPTH_TEST);

    _geo->Draw();

    glEnable(GL_DEPTH_TEST);
}

void Skybox::update(float elapsedTimeMs){
    // Ignore parameter unused
    (void)elapsedTimeMs;
}

void Skybox::MouseClick(glm::vec3 position){
    // Ignore parameter unused
    (void)position;
}

std::string Skybox::getVertexShader(){
    return "res/shader/skybox.vs.glsl";
}

std::string Skybox::getFragmentShader(){
    return "res/shader/skybox.fs.glsl";
}

vec3 Skybox::Position3D(){
    return vec3();
}

void Skybox::recreateGeoemtry(){
    // Box does not change
}
