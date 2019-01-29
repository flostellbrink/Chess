#ifndef CATMULL_ROM_ANIMATION_H
#define CATMULL_ROM_ANIMATION_H

#include <vector>
#include "Animation.h"

template <class T>
class CatmullRomAnimation : public Animation<T>
{
public:
  CatmullRomAnimation(float duration, T& property, std::vector<T> values);
  T ValueAt(float value) override;

protected:
  std::vector<T> values_;
};

// Same as in animation.h
template <class T>
CatmullRomAnimation<T>::CatmullRomAnimation(const float duration, T& property, std::vector<T> values)
  : Animation<T>(duration, property)
{
  auto tanFac = 0.166f;
  if (values.size() > 1)
  {
    T tangent = (values[1] - values[0]) * tanFac;
    values_.push_back(values[0]);
    values_.push_back(values[0]);
    values_.push_back(values[0] + tangent);
  }
  for (auto i = 1; i < static_cast<int>(values.size()) - 1; ++i)
  {
    T tangent = (values[i + 1] - values[i - 1]) * tanFac;
    values_.push_back(values[i] - tangent);
    values_.push_back(values[i]);
    values_.push_back(values[i] + tangent);
  }
  if (values.size() > 1)
  {
    T tangent = (values[1] - values[0]) * tanFac;
    values_.push_back(values[values.size() - 1] - tangent);
    values_.push_back(values[values.size() - 1]);
    values_.push_back(values[values.size() - 1]);
  }
}

template <class T>
T CatmullRomAnimation<T>::ValueAt(float value)
{
  // Figure out what segment we are in
  const auto segments = static_cast<int>(values_.size()) / 3 - 1;
  value *= segments;
  auto segment = static_cast<int>(value);
  if (segment == segments)
    segment--;
  value -= segment;

  auto startIndex = segment * 3 + 1;

  T point1 = values_.at(startIndex), point2 = values_.at(startIndex + 1),
    point3 = values_.at(startIndex + 2), point4 = values_.at(startIndex + 3);

  // Calculate the result using de Casteljau
  return point1 * static_cast<float>(pow(1 - value, 3))
    + point2 * static_cast<float>(3 * pow(1 - value, 2) * value)
    + point3 * static_cast<float>(3 * (1 - value) * pow(value, 2))
    + point4 * static_cast<float>(pow(value, 3));
}

#endif // CATMULL_ROM_ANIMATION_H
