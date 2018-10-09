#include <GL/glew.h>
#include <glm/gtx/transform.hpp>
#include "camera.hpp"
#include "objectManager.hpp"
#include <iostream>
#include "math.h"

using glm::vec2;
using glm::vec3;
using glm::mat4;

const float camSpeed = .01, camZoom = 10, quarterRot = (const float) (glm::pi<float>() / 2.f);

Camera::Camera() : _zoomFactor(1.7), _cameraRotation(0, -.75), _smoothCameraRotation(0, -.75), _autoRotation(), _whiteSide(true)
{

}

vec3 Camera::Position(){
    float Y = _smoothCameraRotation.y + _autoRotation.y,
          X = _smoothCameraRotation.x + _autoRotation.x;
    return camZoom * vec3(sin(Y) * cos(X), cos(Y), sin(X) * sin(Y));
}

vec3 Camera::Up(){
    // This can be simplified to (0,1,0) by restricting the camera movement
    float Y = _smoothCameraRotation.y + _autoRotation.y,
          X = _smoothCameraRotation.x + _autoRotation.x;
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
    auto lightPos = Drawable::_lightPos;
    auto shadowProjection = glm::ortho<float>(-30, 30, -30, 30, 10, 100);
    auto shadowView = glm::lookAt(lightPos, vec3(), vec3(0,1,0));

    return shadowProjection * shadowView;
}

void Camera::Update(){
    _smoothCameraRotation += (_cameraRotation - _smoothCameraRotation) * 0.1f;
}

void Camera::SetBoardSide(bool whiteSide){
    // TODO create delay animation instead of this hack
    float fakeProperty = 0;
    ObjectManager::Animations.PlayLast(new LinearAnimation<float>(500, fakeProperty, 0, 1));

    if(whiteSide == _whiteSide) return;
    _whiteSide = whiteSide;

    vec2 resetPos = vec2(0, -.75);
    _autoRotation += _smoothCameraRotation - resetPos;
    _smoothCameraRotation = _cameraRotation = resetPos;

    // ensure the camera will always rotate along the smaller angle
    if(whiteSide && _autoRotation.x > glm::pi<float>())
        _autoRotation.x -= 2.f * glm::pi<float>();
    if(!whiteSide && _autoRotation.x < 0)
        _autoRotation.x += 2.f * glm::pi<float>();
    ObjectManager::Animations.PlayLast(
                new FadeAnimation<vec2>(1500, _autoRotation, _autoRotation, vec2(whiteSide ? 0 : glm::pi<float>(), 0)));
}

void Camera::MouseDown(){
    _mouseMoving = true;
}

void Camera::MouseUp(){
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
