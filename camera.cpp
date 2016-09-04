#include <GL/glew.h>
#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm/gtx/transform.hpp>
#include "camera.hpp"
#include "objects/objectManager.hpp"
#include <iostream>

#include "math.h"
#ifndef M_PI
#define M_PI glm::pi<float>()
#endif

using glm::vec2;
using glm::vec3;
using glm::mat4;

const float camSpeed = .01, camZoom = 10, quarterRot = M_PI / 2.;

Camera::Camera() : _zoomFactor(1.7), _cameraRotation(0, -.75), _autoRotation(), _whiteSide(true)
{

}

vec3 Camera::Position(){
    float Y = _cameraRotation.y + _autoRotation.y,
          X = _cameraRotation.x + _autoRotation.x;
    return camZoom * vec3(sin(Y) * cos(X), cos(Y), sin(X) * sin(Y));
}

vec3 Camera::Up(){
    // This can be simplified to (0,1,0) by restricting the camera movement
    float Y = _cameraRotation.y + _autoRotation.y,
          X = _cameraRotation.x + _autoRotation.x;
    return camZoom * vec3(sin(Y + .1f) * cos(X), cos(Y + .1f), sin(X) * sin(Y + .1f)) - Position();
}

mat4 Camera::ViewMat(){
    //return glm::lookAt(_zoomFactor * Position() + vec3(0,5,0), vec3(0,5,0), Up());
    return glm::lookAt(_zoomFactor * Position(), vec3(), Up());
}

mat4 Camera::ViewMatAtCamera(){
    return glm::lookAt(vec3(), -Position(), Up());
}

mat4 Camera::ViewProjectionShadow(){
    vec3 up(0,1,0), lightDir = -up, lightPos = Drawable::_lightPos;
    // There is some trick to perspective shadows I don't quite get
    mat4 //shadowProjection = glm::perspective(glm::radians(120.0f), 1.0f, 10.f, 25.0f),
         shadowProjection = glm::ortho<float>(-30,30,-30,30,10,25),
         shadowView = glm::lookAt(lightPos, lightPos + lightDir, vec3(1,0,0));

    return shadowProjection * shadowView;
}

void Camera::Update(float elapsedTimeMs){
    // Ignore parameter unused
    (void)elapsedTimeMs;
}

void Camera::SetBoardSide(bool whiteSide){
    // TODO create delay animation instead of this hack
    float fakeProperty;
    ObjectManager::Instance.Animations.PlayLast(new LinearAnimation<float>(500, fakeProperty, 0, 1));

    if(whiteSide == _whiteSide) return;
    _whiteSide = whiteSide;

    vec2 resetPos = vec2(0, -.75);
    _autoRotation += _cameraRotation - resetPos;
    _cameraRotation = resetPos;

    // ensure the camera will always rotate along the smaller angle
    if(whiteSide && _autoRotation.x > M_PI)
        _autoRotation.x -= 2. * M_PI;
    if(!whiteSide && _autoRotation.x < 0)
        _autoRotation.x += 2. * M_PI;
    ObjectManager::Instance.Animations.PlayLast(
                new FadeAnimation<vec2>(1500, _autoRotation, _autoRotation, vec2(whiteSide ? 0 : M_PI, 0)));
}

void Camera::MouseDown(vec2 mousePos){
    _mouseMoving = true;
    _oldMouse = mousePos;
}

void Camera::MouseUp(vec2 mousePos){
    // Ignore parameter unused
    (void)mousePos;

    _mouseMoving = false;
}

void Camera::MouseMove(vec2 mousePos){
    if(_mouseMoving)
        _cameraRotation += (mousePos - _oldMouse) * camSpeed;
    _oldMouse = mousePos;

    if(_cameraRotation.x > quarterRot)
        _cameraRotation.x = quarterRot;
    if(_cameraRotation.x < -quarterRot)
        _cameraRotation.x = -quarterRot;
    if(_cameraRotation.y > 0)
        _cameraRotation.y = 0;
    if(_cameraRotation.y < -quarterRot)
        _cameraRotation.y = -quarterRot;
}

void Camera::MouseWheel(float wheelDelta){
    _zoomFactor *= std::pow(1.1f, wheelDelta);

    /*if(_zoomFactor < 1)
        _zoomFactor = 1;*/
    if (_zoomFactor > 7)
        _zoomFactor = 7;
}
