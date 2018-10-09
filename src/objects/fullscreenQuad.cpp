#include <utility>

#include <GL/glew.h>

#include "fullscreenQuad.hpp"
#include <iostream>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "objectManager.hpp"
#include "src/config.h"
#include "src/objects.hpp"
#include "math.h"
#include "src/logger.h"

using glm::value_ptr;
using glm::vec3;
using glm::mat4;

FullscreenQuad::FullscreenQuad(string vertexShader, string fragmentShader)
    :Drawable(objects::FullscreenQuad)
{
    _vertexShader = std::move(vertexShader);
    _fragmentShader = std::move(fragmentShader);
}

void FullscreenQuad::init(){
    Drawable::init();
    recreateGeoemtry();
}

void FullscreenQuad::recreateGeoemtry(){
    delete _geo;
    _geo = ObjectManager::Geos.GetGeometry(_objectId);
}

void FullscreenQuad::draw(glm::mat4 projection_matrix){
    // Ignore parameter unused
    (void)projection_matrix;

    if(_program == nullptr){
        Logger::error("program not loaded");
    }
    _program->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, PostTexture);
    _program->bind(0, "tex");
    _program->bind(OverlayOpacity, "overlayOpacity");

    vec2 overlaySize(2.f, 1.f);
    float overlayRatio = overlaySize.x / overlaySize.y, viewPortRatio = (float)Config::viewportWidth / (float)Config::viewportHeight;
    vec2 fac = viewPortRatio < overlayRatio
            ? vec2(1.f, overlayRatio / viewPortRatio)
            : vec2(viewPortRatio / overlayRatio , 1.f);
    fac *= 1.5f;
    if (_program->hasUniform("overlayFac")) {
        _program->bind(fac, "overlayFac");
    }

    // Make sure textures are loaded before needed, popping and frame drop are not nice!
    ObjectManager::Textures.Texture(_objectId);
    if (_program->hasUniform("texOverlay")) {
        if (OverlayOpacity > 0 && OverlayState >= 0) {
            ObjectManager::Textures.Texture(_objectId)[OverlayState]->Bind(GL_TEXTURE_2D, 1);
            _program->bind(1, "texOverlay");
            _program->bind(1, "texOverlayEnabled");
        }
        else {
            _program->bind(0, "texOverlayEnabled");
        }
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
    return _vertexShader;
}

std::string FullscreenQuad::getFragmentShader()
{
    return _fragmentShader;
}

vec3 FullscreenQuad::Position3D(){
    return vec3();
}
