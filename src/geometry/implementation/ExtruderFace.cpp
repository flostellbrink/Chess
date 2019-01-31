#include <src/Config.h>
#include <glm/glm.hpp>
#include "ExtruderFace.h"
#include "src/curve/Curve.h"


ExtruderFace::ExtruderFace(Curve* profileCurve,
                           Curve* compressedProfile,
                           Curve* widthCurve,
                           const float widthPos,
                           const float scale) : scale_(scale)
{
  profile_curve_ = profileCurve;
  compressed_profile_ = compressedProfile;
  width_curve_ = widthCurve;
  width_pos_ = widthPos;
}

void ExtruderFace::Create()
{
  const auto texScale = 0.1f;
  auto profilePoints = profile_curve_->InterpolatedPoints(Config::geo_resolution2);
  auto compressedPoints = compressed_profile_->InterpolatedPoints(Config::geo_resolution2);

  const auto width = width_curve_->InterpolatedPoint(width_pos_);
  const auto widthTangent = normalize(glm::vec3(0, 0, width_pos_ - 0.5f));

  const auto profileRes = static_cast<int>(profilePoints.size());
  const auto compressedRes = static_cast<int>(compressedPoints.size());
  assert(profileRes == compressedRes);

  for (auto i = 0; i < profileRes; ++i)
  {
    const auto profile = profilePoints[i];
    positions_.push_back(scale_ * glm::vec3(profile.x * width.y, profile.y, width.x));
    texture_coordinates_.push_back(texScale * scale_ * profile);
    normals_.push_back(widthTangent);
  }

  for (auto i = 0; i < compressedRes; ++i)
  {
    const auto profile = compressedPoints[i];
    positions_.push_back(scale_ * glm::vec3(profile.x * width.y, profile.y, width.x));
    texture_coordinates_.push_back(texScale * scale_ * profile);
    normals_.push_back(widthTangent);
  }

  for (auto i = 0; i < compressedRes - 1; ++i)
  {
    AddIndex(static_cast<unsigned int &&>(i + 0));
    AddIndex(static_cast<unsigned int &&>(i + 1));
    AddIndex(static_cast<unsigned int &&>(i + 0 + profileRes));
    AddIndex(static_cast<unsigned int &&>(i + 1));
    AddIndex(static_cast<unsigned int &&>(i + 1 + profileRes));
    AddIndex(static_cast<unsigned int &&>(i + 0 + profileRes));
  }
}
