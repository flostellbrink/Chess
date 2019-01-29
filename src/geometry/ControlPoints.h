// Loads geometry for all objects
#ifndef CONTROL_POINTS_H
#define CONTROL_POINTS_H

#include <vector>
#include <glm/glm.hpp>

class ControlPoints
{
public:
  ControlPoints();
  void SetTheme(int themeId);
  std::vector<std::vector<glm::vec2>> GetControlPoints2D(int objectId) const;

protected:
  int theme_id_;
};

#endif // CONTROL_POINTS_H
