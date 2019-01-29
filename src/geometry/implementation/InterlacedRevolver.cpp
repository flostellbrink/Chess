#include <glm/glm.hpp>
#include "InterlacedRevolver.h"
#include "src/Config.h"
#include "src/curve/Curve.h"


InterlacedRevolver::InterlacedRevolver(Curve* baseCurve, Curve* heightCurve1, Curve* heightCurve2, float length1, float length2, float scale)
  : base_curve_(baseCurve), height_curve1_(heightCurve1), height_curve2_(heightCurve2), scale_(scale), length1_(length1), length2_(length2) {

}

void InterlacedRevolver::Create() {
  // Texture needs to seamlessly wrap around
  const auto texWidth = static_cast<int>(scale_ * 4 + 1);

  auto basePoints = base_curve_->InterpolatedPoints(Config::geo_resolution1);
  auto baseTangent = base_curve_->InterpolatedTangents();

  const auto baseRes = static_cast<int>(basePoints.size());
  for (auto j = 0; j < baseRes; ++j) {
    const auto base = basePoints[j];

    // Value keeps track of the rotation to switch between curves
    // Offset to avoid straight edges
    auto value = static_cast<float>(j) / static_cast<float>(baseRes) + length1_ / 0.2f;
    while (value > length1_ + length2_) {
      value -= length1_ + length2_;
    }

    auto heightCurve = value < length1_ ? height_curve1_ : height_curve2_;
    auto heightPoints = heightCurve->InterpolatedPoints(Config::geo_resolution2);
    auto heightTangent = heightCurve->InterpolatedTangents();

    const auto heightRes = static_cast<int>(heightPoints.size());
    for (auto i = 0; i < heightRes; ++i) {
      const auto height = heightPoints[i];

      const glm::vec3 position(height.x * base.x, height.y, height.x * base.y);
      const glm::vec2 texture(
        static_cast<float>(j) / (static_cast<float>(baseRes) - 1) * static_cast<float>(texWidth),
        i == 0 ? 0 : length(heightPoints[i - 1] - height) * scale_ / 5);
      const auto normal = i < heightRes - 1
        ? cross(
          glm::vec3(base.x * heightTangent[i].x, heightTangent[i].y, base.y * heightTangent[i].x),
          glm::vec3(baseTangent[j].x, 0, baseTangent[j].y))
        : glm::vec3(0, 1, 0);

      positions_.push_back(scale_ * position);
      texture_coordinates_.emplace_back(texture);
      normals_.push_back(normalize(normal));

      if (i < heightRes - 1 && j < baseRes - 1) {
        AddIndex(i + 0 + heightRes * (j + 0));
        AddIndex(i + 0 + heightRes * (j + 1));
        AddIndex(i + 1 + heightRes * (j + 0));
        AddIndex(i + 0 + heightRes * (j + 1));
        AddIndex(i + 1 + heightRes * (j + 1));
        AddIndex((i + 1) + heightRes * (j + 0));
      }
    }
  }
}
