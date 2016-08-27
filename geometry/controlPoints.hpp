// Loads geometry for all objects
#ifndef CONTROLPOINTS_H
#define CONTROLPOINTS_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include <vector>
#include <glm/common.hpp>

using std::vector;
using glm::vec2;
using glm::vec3;

class ControlPoints {
    public:
    ControlPoints();
    void SetTheme(int themeID);
    vector<vector<vec2>> GetControlPoints2D(int objectsID);

    protected:
    int _themeID;

    private:
};

#endif // CONTROLPOINTS_H
