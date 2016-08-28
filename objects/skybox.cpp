#include <GL/glew.h>
#include <GL/gl.h>

#include "skybox.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "glbase/gltool.hpp"

#include "vector"
#include "objects.hpp"
#include "objectManager.hpp"


Skybox::Skybox(): Drawable(objects::Skybox) {
	
}

void Skybox::init(){
    Drawable::init();
	_geo = ObjectManager::Geos.GetCube();
}

void Skybox::draw(glm::mat4 projection_matrix){   

}

void Skybox::drawShadow(glm::mat4 projection_matrix){

}

void Skybox::drawSpecular(glm::mat4 projection_matrix){

}

void Skybox::drawSkybox(glm::mat4 projection_matrix){
    // Load program
    glUseProgram(_program);

    ObjectManager::Textures.Texture(_objectId)[0]->Bind(GL_TEXTURE_CUBE_MAP);

    // set parameter
    glUniformMatrix4fv(glGetUniformLocation(_program, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(glGetUniformLocation(_program, "modelview_matrix"), 1, GL_FALSE, glm::value_ptr(_modelViewMatrix));

    glDisable(GL_DEPTH_TEST);

    _geo->Draw();

    glEnable(GL_DEPTH_TEST);
}

void Skybox::update(float elapsedTimeMs){
    // Static
}

void Skybox::MouseClick(glm::vec3 position){
    // Ignore
}

std::string Skybox::getVertexShader(){
    return Drawable::loadShaderFile(":/shader/skybox.vs.glsl");
}

std::string Skybox::getFragmentShader(){
    return Drawable::loadShaderFile(":/shader/skybox.fs.glsl");
}

vec3 Skybox::Position3D(){
    return vec3();
}

void Skybox::recreateGeoemtry(){
    // Box does not change
}
