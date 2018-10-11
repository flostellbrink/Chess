#ifndef CURVE_H
#define CURVE_H

#include <vector>
#include <glm/glm.hpp>

class Curve {
public:
  virtual ~Curve() = default;
  Curve();
  /**
   * @brief InterpolatedPoints generates points between control points
   * @param resolution steps between control points
   * @return vector of points on the curve. result size may vary
   */
  virtual std::vector<glm::vec2> InterpolatedPoints(int resolution = 10) = 0;

  /**
   * @brief InterpolatedPoints calculates an interpolated point
   * @param value position between 0 and 1 on the curve
   * @return interpolated point
   */
  virtual glm::vec2 InterpolatedPoint(float value) = 0;

  /**
   * @brief InterpolatedTangents calculates tangents between control points, takes resolution from last InterpolatedPoints call
   * @return vector of tangents on the curve. result size may vary
   */
  virtual std::vector<glm::vec2> InterpolatedTangents() = 0;

  /**
   * @brief InterpolatedTangent calculates an interpolted tangent
   * @param value position between 0 and 1 on the curve
   * @return interpolates tangent
   */
  virtual glm::vec2 InterpolatedTangent(float value) = 0;

protected:
  std::vector<glm::vec2> tangents_;
};


#endif // CURVE_H
