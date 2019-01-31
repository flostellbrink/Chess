#include <glm/glm.hpp>

#include "Extruder.h"
#include "src/Config.h"
#include "src/curve/Curve.h"

Extruder::Extruder(Curve* profileCurve, Curve* widthCurve, const float scale) : scale_(scale)
{
  profile_curve_ = profileCurve;
  width_curve_ = widthCurve;
}

void Extruder::Create()
{
  auto profilePoints = profile_curve_->InterpolatedPoints(Config::geo_resolution2);
  auto profileTangents = profile_curve_->InterpolatedTangents();
  auto widthPoints = width_curve_->InterpolatedPoints(resolution_);
  auto widthTangents = width_curve_->InterpolatedTangents();
  const auto profileRes = static_cast<int>(profilePoints.size());
  const auto widthRes = static_cast<int>(widthPoints.size());

  for (auto i = 0; i < widthRes; ++i)
  {
    const auto width = widthPoints[i];
    for (auto j = 0; j < profileRes; ++j)
    {
      const auto profile = profilePoints[j];
      const glm::vec3 position(profile.x * width.y, profile.y, width.x);
      const glm::vec2 texture(width.x * scale_, j == 0 ? 0 : length(profilePoints[j - 1] - profile) * scale_);
      const auto normal = cross(
        glm::vec3(profileTangents[j].x, profileTangents[j].y, 0),
        glm::vec3(0, 0, widthTangents[i].x));

      positions_.push_back(scale_ * position);
      texture_coordinates_.emplace_back(texture);
      normals_.push_back(normalize(normal));
    }
  }

  for (auto i = 0; i < widthRes - 1; ++i)
  {
    for (auto j = 0; j < profileRes - 1; ++j)
    {
      AddIndex(static_cast<unsigned int &&>((i + 0) * profileRes + (j + 0)));
      AddIndex(static_cast<unsigned int &&>((i + 0) * profileRes + (j + 1)));
      AddIndex(static_cast<unsigned int &&>((i + 1) * profileRes + (j + 0)));
      AddIndex(static_cast<unsigned int &&>((i + 0) * profileRes + (j + 1)));
      AddIndex(static_cast<unsigned int &&>((i + 1) * profileRes + (j + 1)));
      AddIndex(static_cast<unsigned int &&>((i + 1) * profileRes + (j + 0)));
    }
  }
}
