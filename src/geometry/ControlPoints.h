// Loads geometry for all objects
#ifndef CONTROLPOINTS_H
#define CONTROLPOINTS_H

#include <vector>
#include <glm/glm.hpp>

class ControlPoints {
public:
  ControlPoints();
  void SetTheme(int themeID);
  std::vector<std::vector<glm::vec2>> GetControlPoints2D(int objectsId) const;

protected:
  int theme_id_;
};

#endif // CONTROLPOINTS_H
