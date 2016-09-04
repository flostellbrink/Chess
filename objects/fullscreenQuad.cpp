#include <GL/glew.h>

#include "fullscreenQuad.hpp"
#include <iostream>
#include "gltool.hpp"
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "objectManager.hpp"
#include "gui/config.h"
#include "objects.hpp"

#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include "math.h"
#ifndef M_PI
#define M_PI glm::pi<float>()
#endif

using glm::value_ptr;
using glm::vec3;
using glm::mat4;

FullscreenQuad::FullscreenQuad(string vertexShader, string fragmentShader)
    :Drawable(objects::FullscreenQuad)
{
    _vertexShader = vertexShader;
    _fragmentShader = fragmentShader;
}

void FullscreenQuad::init(){
    Drawable::init();
    recreateGeoemtry();
}

void FullscreenQuad::recreateGeoemtry(){
    if(_geo)
        delete _geo;
    _geo = ObjectManager::Geos.GetGeometry(_objectId);
}

void FullscreenQuad::draw(glm::mat4 projection_matrix){
    // Ignore parameter unused
    (void)projection_matrix;

    if(_program == 0){
        std::cerr << "program not loaded" << std::endl;
    }
    glUseProgram(_program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, PostTexture);
    glUniform1i(glGetUniformLocation(_program, "tex"), 0);
    glUniform1f(glGetUniformLocation(_program, "overlayOpacity"), OverlayOpacity);

    vec2 overlaySize(2.f, 1.f);
    float overlayRatio = overlaySize.x / overlaySize.y, viewPortRatio = (float)Config::viewportWidth / (float)Config::viewportHeight;
    vec2 fac = viewPortRatio < overlayRatio
            ? vec2(1.f, overlayRatio / viewPortRatio)
            : vec2(viewPortRatio / overlayRatio , 1.f);
    fac *= 1.5f;
    glUniform2fv(glGetUniformLocation(_program, "overlayFac"), 1, glm::value_ptr(fac));

    // Make sure textures are loaded before needed, popin and framedrop is not nice..
    ObjectManager::Textures.Texture(_objectId);
    if(OverlayOpacity > 0 && OverlayState >= 0){
        ObjectManager::Textures.Texture(_objectId)[OverlayState]->Bind(GL_TEXTURE_2D, 1);
        glUniform1i(glGetUniformLocation(_program, "texOverlay"), 1);
        glUniform1i(glGetUniformLocation(_program, "texOverlayEnabled"), 1);
    } else {
        glUniform1i(glGetUniformLocation(_program, "texOverlayEnabled"), 0);
    }

    _geo->Draw();
}

void FullscreenQuad::drawShadow(glm::mat4 projection_matrix){
    // Ignore parameter unused
    (void)projection_matrix;
}

void FullscreenQuad::update(float elapsedTimeMs){
    // Ignore parameter unused
    (void)elapsedTimeMs;
}

void FullscreenQuad::MouseClick(glm::vec3 position){
    // Ignore parameter unused
    (void)position;
}

std::string FullscreenQuad::getVertexShader()
{
    return Drawable::loadShaderFile(_vertexShader);
}

std::string FullscreenQuad::getFragmentShader()
{
    return Drawable::loadShaderFile(_fragmentShader);
}

vec3 FullscreenQuad::Position3D(){
    return vec3();
}
