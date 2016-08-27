#include <GL/glew.h>

#include "fixedPointsCurve.hpp"

FixedPointsCurve::FixedPointsCurve(ControlPoints controlPoints, int curveID, int objectID){
    _controlPoints = controlPoints;
    _curveID = curveID;
    _objectID = objectID;
}

/**
 * Very inefficient. Do not call every frame
 */
vector<vec2> FixedPointsCurve::InterpolatedPoints(int resolution){
     vector<vec2> controlPoints = _controlPoints.GetControlPoints2D(_objectID)[_curveID];
     _tangents.clear();
     _tangents.push_back(vec2(1, 0));
     for (int i = 1; i < controlPoints.size() - 1; ++i)
         _tangents.push_back(controlPoints[i+1] - controlPoints[i-1]);
     _tangents.push_back(vec2(-1,0 ));
     return controlPoints;
}

/**
 * Extremely inefficient. Do not use to evaluate entire curve
 */
vec2 FixedPointsCurve::InterpolatedPoint(float value){
    vector<vec2> controlPoints = InterpolatedPoints(0);
    value *= controlPoints.size();
    return controlPoints[value];
}

/**
 * Could be improved by storing points from InterpolatedPoints
 */
vector<vec2> FixedPointsCurve::InterpolatedTangents(){
    return _tangents;
}

/**
 * Extremely inefficient. Do not use to evaluate entire curve
 */
vec2 FixedPointsCurve::InterpolatedTangent(float value){
    vector<vec2> controlPoints = InterpolatedPoints(0);
    value *= controlPoints.size();
    int index = value;
    if(index == 0 || index == controlPoints.size() -1)
        return vec2();
    return controlPoints[index+1] - controlPoints[index-1];
}
