// Loads geometry for all objects
// Also handles recreation on reolution/ theme change
#ifndef GEOMANAGER_H
#define GEOMANAGER_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include <vector>
#include "geometry.hpp"
#include "controlPoints.hpp"

using std::vector;

class GeoManager{
    public:
    GeoManager();
    void SetTheme(int themeID);
    void Regenerate();
    void Remove(Geometry* geo);

    Geometry* GetSquare();
    Geometry* GetCube();
    Geometry* GetGeometry(int objectID);

    protected:
    Geometry* AddAndRecreate(Geometry* geo);
    Geometry* GetRevolver(int objectID);
    Geometry* GetRevolver(int objectID, float scale);
    Geometry* GetInterlacedRevolver(int objectID, float length1, float length2);
    Geometry* GetRevolverAndExtruded(int objectID, float scale = .1f, float radius = .2f);
    Geometry* GetExtruded(int objectID, float scale = .1f, float radius = .2f);

    int _themeID;
    vector<Geometry*> _geos;
    ControlPoints _controlPoints;

    private:
};


#endif // GEOMANAGER_H
