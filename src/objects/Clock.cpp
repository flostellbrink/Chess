#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.inl>

#include "Clock.h"
#include "src/Objects.h"
#include "ObjectManager.h"
#include "BasicObject.h"

BasicObject* rotate_x(BasicObject* object, const float rotation)
{
  object->SetRotationX(rotation);
  return object;
}

BasicObject* rotate_z(BasicObject* object, const float rotation)
{
  object->SetRotationZ(rotation);
  return object;
}

Clock::Clock()
{
  white_time_ = black_time_ = start_time_;
  auto manager = &ObjectManager::instance;

  // Clock case
  manager->AddObject(new BasicObject(objects::clock_base, glm::vec3(0, -1, 18)));
  manager->AddObject(new BasicObject(objects::clock_leg, glm::vec3(4, -1, 18.5f)));
  manager->AddObject(new BasicObject(objects::clock_leg, glm::vec3(-4, -1, 18.5f)));
  manager->AddObject(new BasicObject(objects::clock_leg, glm::vec3(4, -1, 17.5f)));
  manager->AddObject(new BasicObject(objects::clock_leg, glm::vec3(-4, -1, 17.5f)));

  // Clock parts for black
  black_clock_button_ = new BasicObject(objects::clock_button, glm::vec3(4, 5.5f, 18));
  black_minutes_hand_ = new BasicObject(objects::black_minutes_hand, glm::vec3(3.4f, 3, 16.8f), 0, "noLight");
  black_seconds_hand_ = new BasicObject(objects::black_seconds_hand, glm::vec3(3.4f, 3, 16.8f), 0, "noLight");
  manager->AddObject(black_clock_button_);
  manager->AddObject(black_minutes_hand_);
  manager->AddObject(black_seconds_hand_);
  manager->AddObject(new BasicObject(objects::clock_button_border, glm::vec3(4, 6, 18)));
  manager->AddObject(rotate_x(new BasicObject(objects::clock_glass_border, glm::vec3(3.4f, 3, 17)),
                              glm::pi<float>() / -2.0f));
  manager->AddObject(rotate_x(new BasicObject(objects::clock_face, glm::vec3(3.4f, 3, 16.9f), 0, "noLight"),
                              glm::pi<float>() / -2.0f));
  manager->AddObject(rotate_z(new BasicObject(objects::clock_time_cap, glm::vec3(3.4f, 3, 16.8f), 0, "noLight"),
                              2 * glm::pi<float>() * (black_time_ / 3600000)));

  /* Clock parts for WHITE */
  white_clock_button_ = new BasicObject(objects::clock_button, glm::vec3(-4, 6, 18));
  white_minutes_hand_ = new BasicObject(objects::white_minutes_hand, glm::vec3(-3.4f, 3, 16.8f), 0, "noLight");
  white_seconds_hand_ = new BasicObject(objects::white_seconds_hand, glm::vec3(-3.4f, 3, 16.8f), 0, "noLight");
  manager->AddObject(white_clock_button_);
  manager->AddObject(white_minutes_hand_);
  manager->AddObject(white_seconds_hand_);
  manager->AddObject(new BasicObject(objects::clock_button_border, glm::vec3(-4, 6, 18)));
  manager->AddObject(rotate_x(new BasicObject(objects::clock_glass_border, glm::vec3(-3.4f, 3, 17)),
                              glm::pi<float>() / -2.0f));
  manager->AddObject(rotate_x(new BasicObject(objects::clock_face, glm::vec3(-3.4f, 3, 16.9f), 0, "noLight"),
                              glm::pi<float>() / -2.0f));
  manager->AddObject(rotate_z(new BasicObject(objects::clock_time_cap, glm::vec3(-3.4f, 3, 16.8f), 0, "noLight"),
                              2 * glm::pi<float>() * (white_time_ / 3600000)));
}

float Clock::GetHandRotation(const float time, const int secondsPerRotation) const
{
  const auto msPerRotation = static_cast<float>(secondsPerRotation * 1000);
  return -2 * glm::pi<float>() * (time - start_time_) / msPerRotation;
}

void Clock::Update(const float elapsedTimeMs, const bool whiteTurn)
{
  time_counter_ += elapsedTimeMs;

  if (whiteTurn != white_on_clock_)
  {
    white_on_clock_ = whiteTurn;
    SwitchPlayer(whiteTurn);
  }

  // Delay cap is hit, start reducing time from the active player's clock
  if (time_counter_ < delay_)
  {
    return;
  }

  if (white_on_clock_)
  {
    if (white_time_ > 0)
    {
      // White still has time remaining, reduce time and move clock hands
      white_time_ -= elapsedTimeMs;
      white_seconds_hand_->SetRotationZ(GetHandRotation(white_time_, 60));
      white_minutes_hand_->SetRotationZ(GetHandRotation(white_time_, 3600));
    }
    else
    {
      // White has no more time, set timeout status indicating black wins
      timeout_status_ = 3;
    }
  }
  else
  {
    if (black_time_ > 0)
    {
      black_time_ -= elapsedTimeMs;
      black_seconds_hand_->SetRotationZ(GetHandRotation(black_time_, 60));
      black_minutes_hand_->SetRotationZ(GetHandRotation(black_time_, 3600));
    }
    else
    {
      timeout_status_ = 2;
    }
  }
}

int Clock::Timeout() const
{
  return timeout_status_;
}

void Clock::SwitchPlayer(const bool whiteTurn)
{
  time_counter_ = 0.f;

  if (whiteTurn)
  {
    black_clock_button_->Position(glm::vec3(4, 5.5f, 18));
    white_clock_button_->Position(glm::vec3(-4, 6, 18));
  }
  else
  {
    black_clock_button_->Position(glm::vec3(4, 6, 18));
    white_clock_button_->Position(glm::vec3(-4, 5.5f, 18));
  }
}
