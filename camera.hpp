#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include "glm/glm.hpp"

using glm::vec2;
using glm::vec3;
using glm::mat4;


class Camera{
    public:
    Camera();
    void Update();
    void MouseDown(vec2 mousePos);
    void MouseUp(vec2 mousePos);
    void MouseMove(vec2 mousePos);
    // 1 corresponds to 1 revolution
    void MouseWheel(float wheelDelta);
    void SetBoardSide(bool whiteSide);
    mat4 ViewMat();
    mat4 ViewMatAtCamera();
    mat4 ViewProjectionShadow();
    vec3 Position();
    vec3 Up();

    vec2 _oldMouse;

    protected:

    private:
    float _zoomFactor;
    vec2 _cameraRotation, _smoothCameraRotation, _autoRotation;
    bool _mouseMoving, _whiteSide;

};


#endif // CAMERA_H
