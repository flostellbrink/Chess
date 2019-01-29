#include <glm/glm.hpp>
#include "Revolver.h"
#include "src/Config.h"
#include "src/curve/Curve.h"


Revolver::Revolver(Curve* baseCurve, Curve* heightCurve, const float scale, const bool clockTexture)
  : base_curve_(baseCurve), height_curve_(heightCurve), scale_(scale), clock_texture_(clockTexture)
{
}

void Revolver::Create()
{
  // Texture needs to seamlessly wrap around
  const auto texWidth = static_cast<int>(scale_ * 4 + 1);

  auto basePoints = base_curve_->InterpolatedPoints(Config::geo_resolution1);
  auto heightPoints = height_curve_->InterpolatedPoints(Config::geo_resolution2);
  auto baseTangent = base_curve_->InterpolatedTangents();
  auto heightTangent = height_curve_->InterpolatedTangents();

  const auto baseRes = static_cast<int>(basePoints.size());
  const auto heightRes = static_cast<int>(heightPoints.size());
  for (auto i = 0; i < heightRes; ++i)
  {
    const auto height = heightPoints[i];
    for (auto j = 0; j < baseRes; ++j)
    {
      const auto base = basePoints[j];

      const glm::vec3 position(height.x * base.x, height.y, height.x * base.y);
      const auto texture = clock_texture_
                             ? glm::vec2(height.x * base.x / -2.f + 0.5f, height.x * base.y / -2.f + 0.5f)
                             : glm::vec2(
                               static_cast<float>(j) / (static_cast<float>(baseRes) - 1) * static_cast<float>(texWidth),
                               i == 0 ? 0 : length(heightPoints[i - 1] - height) * scale_ / 5);
      const auto normal = i < heightRes - 1
                            ? cross(
                              glm::vec3(base.x * heightTangent[i].x, heightTangent[i].y, base.y * heightTangent[i].x),
                              glm::vec3(baseTangent[j].x, 0, baseTangent[j].y))
                            : glm::vec3(0, 1, 0);

      positions_.push_back(scale_ * position);
      texture_coordinates_.push_back(texture);
      normals_.push_back(normalize(normal));

      if (i < heightRes - 1 && j < baseRes - 1)
      {
        AddIndex(static_cast<unsigned int &&>((i + 0) * baseRes + (j + 0)));
        AddIndex(static_cast<unsigned int &&>((i + 0) * baseRes + (j + 1)));
        AddIndex(static_cast<unsigned int &&>((i + 1) * baseRes + (j + 0)));
        AddIndex(static_cast<unsigned int &&>((i + 0) * baseRes + (j + 1)));
        AddIndex(static_cast<unsigned int &&>((i + 1) * baseRes + (j + 1)));
        AddIndex(static_cast<unsigned int &&>((i + 1) * baseRes + (j + 0)));
      }
    }
  }
}
