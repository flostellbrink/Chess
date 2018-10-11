#include <src/Config.h>
#include <glm/glm.hpp>
#include "ExtruderFace.h"
#include "src/curve/Curve.h"


ExtruderFace::ExtruderFace(Curve* profileCurve, Curve* compressedProfile, Curve* widthCurve, float widthPos, float scale) : scale_(scale) {
  profile_curve_ = profileCurve;
  compressed_profile_ = compressedProfile;
  width_curve_ = widthCurve;
  width_pos_ = widthPos;
}

void ExtruderFace::Create() {
  const auto texScale = 0.1f;
  auto profilePoints = profile_curve_->InterpolatedPoints(Config::geo_resolution2);
  auto compressedPoints = compressed_profile_->InterpolatedPoints(Config::geo_resolution2);

  const auto width = width_curve_->InterpolatedPoint(width_pos_);
  const auto widthTangent = normalize(glm::vec3(0, 0, width_pos_ - 0.5f));

  const auto profileRes = static_cast<int>(profilePoints.size());
  const auto compressedRes = static_cast<int>(compressedPoints.size());
  assert(profileRes == compressedRes);

  for (auto j = 0; j < profileRes; ++j) {
    const auto profile = profilePoints[j];
    positions_.push_back(scale_ * glm::vec3(profile.x * width.y, profile.y, width.x));
    texture_coordinates_.push_back(texScale * scale_ * profile);
    normals_.push_back(widthTangent);
  }

  for (auto j = 0; j < compressedRes; ++j) {
    const auto profile = compressedPoints[j];
    positions_.push_back(scale_ * glm::vec3(profile.x * width.y, profile.y, width.x));
    texture_coordinates_.push_back(texScale * scale_ * profile);
    normals_.push_back(widthTangent);

    AddIndex(static_cast<unsigned int &&>(j + 0));
    AddIndex(static_cast<unsigned int &&>(j + 1));
    AddIndex(static_cast<unsigned int &&>(j + 0 + profileRes));
    AddIndex(static_cast<unsigned int &&>(j + 1));
    AddIndex(static_cast<unsigned int &&>(j + 1 + profileRes));
    AddIndex(static_cast<unsigned int &&>(j + 0 + profileRes));
  }
}
