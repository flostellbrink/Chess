#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/constants.inl>

#include "Clock.h"
#include "src/Objects.h"
#include "ObjectManager.h"
#include "BasicObject.h"

BasicObject* rotate_x(BasicObject* object, float rotation)
{
  object->SetRotationX(rotation);
  return object;
}

BasicObject* rotate_z(BasicObject* object, float rotation)
{
  object->SetRotationZ(rotation);
  return object;
}

Clock::Clock() {
  whiteTime = blackTime = startTime;
  auto manager = &ObjectManager::instance;

  // Clock case
  manager->AddObject(new BasicObject(objects::clock_base, glm::vec3(0, -1, 18)));
  manager->AddObject(new BasicObject(objects::clock_leg, glm::vec3(4, -1, 18.5f)));
  manager->AddObject(new BasicObject(objects::clock_leg, glm::vec3(-4, -1, 18.5f)));
  manager->AddObject(new BasicObject(objects::clock_leg, glm::vec3(4, -1, 17.5f)));
  manager->AddObject(new BasicObject(objects::clock_leg, glm::vec3(-4, -1, 17.5f)));

  // Clock parts for black
  blackClockButton_ = new BasicObject(objects::clock_button, glm::vec3(4, 5.5f, 18));
  blackMinutesHand_ = new BasicObject(objects::black_minutes_hand, glm::vec3(3.4f, 3, 16.8f), 0, "noLight");
  blackSecondsHand_ = new BasicObject(objects::black_seconds_hand, glm::vec3(3.4f, 3, 16.8f), 0, "noLight");
  manager->AddObject(blackClockButton_);
  manager->AddObject(blackMinutesHand_);
  manager->AddObject(blackSecondsHand_);
  manager->AddObject(new BasicObject(objects::clock_button_border, glm::vec3(4, 6, 18)));
  manager->AddObject(rotate_x(new BasicObject(objects::clock_glass_border, glm::vec3(3.4f, 3, 17)), glm::pi<float>() / -2.0f));
  manager->AddObject(rotate_x(new BasicObject(objects::clock_face, glm::vec3(3.4f, 3, 16.9f), 0, "noLight"), glm::pi<float>() / -2.0f));
  manager->AddObject(rotate_z(new BasicObject(objects::clock_time_cap, glm::vec3(3.4f, 3, 16.8f), 0, "noLight"), 2 * glm::pi<float>() * (blackTime / 3600000)));

  /* Clock parts for WHITE */
  whiteClockButton_ = new BasicObject(objects::clock_button, glm::vec3(-4, 6, 18));
  whiteMinutesHand_ = new BasicObject(objects::white_minutes_hand, glm::vec3(-3.4f, 3, 16.8f), 0, "noLight");
  whiteSecondsHand_ = new BasicObject(objects::white_seconds_hand, glm::vec3(-3.4f, 3, 16.8f), 0, "noLight");
  manager->AddObject(whiteClockButton_);
  manager->AddObject(whiteMinutesHand_);
  manager->AddObject(whiteSecondsHand_);
  manager->AddObject(new BasicObject(objects::clock_button_border, glm::vec3(-4, 6, 18)));
  manager->AddObject(rotate_x(new BasicObject(objects::clock_glass_border, glm::vec3(-3.4f, 3, 17)), glm::pi<float>() / -2.0f));
  manager->AddObject(rotate_x(new BasicObject(objects::clock_face, glm::vec3(-3.4f, 3, 16.9f), 0, "noLight"), glm::pi<float>() / -2.0f));
  manager->AddObject(rotate_z(new BasicObject(objects::clock_time_cap, glm::vec3(-3.4f, 3, 16.8f), 0, "noLight"), 2 * glm::pi<float>() * (whiteTime / 3600000)));
}

float Clock::getHandRotation(const float time, const int secondsPerRotation) const
{
  const auto msPerRotation = static_cast<float>(secondsPerRotation * 1000);
  return -2 * glm::pi<float>() * (time - startTime) / msPerRotation;
}

void Clock::Update(const float elapsedTimeMs, const bool whiteTurn) {
  timeCounter += elapsedTimeMs;

  if (whiteTurn != whiteOnClock_) {
    whiteOnClock_ = whiteTurn;
    SwitchPlayer(whiteTurn);
  }

  // Delay cap is hit, start reducing time from the active player's clock
  if (timeCounter < delay) {
    return;
  }

  if (whiteOnClock_) {
    if (whiteTime > 0) {
      // White still has time remaining, reduce time and move clock hands
      whiteTime -= elapsedTimeMs;
      whiteSecondsHand_->SetRotationZ(getHandRotation(whiteTime, 60));
      whiteMinutesHand_->SetRotationZ(getHandRotation(whiteTime, 3600));
    }
    else {
      // White has no more time, set timeout status indicating black wins
      timeoutStatus_ = 3;
    }

  }
  else {
    if (blackTime > 0) {
      blackTime -= elapsedTimeMs;
      blackSecondsHand_->SetRotationZ(getHandRotation(blackTime, 60));
      blackMinutesHand_->SetRotationZ(getHandRotation(blackTime, 3600));
    }
    else {
      timeoutStatus_ = 2;
    }
  }
}

int Clock::Timeout() const
{
  return timeoutStatus_;
}

void Clock::SwitchPlayer(const bool whiteTurn) {
  timeCounter = 0.f;

  if (whiteTurn) {
    blackClockButton_->Position(glm::vec3(4, 5.5f, 18));
    whiteClockButton_->Position(glm::vec3(-4, 6, 18));
  }
  else {
    blackClockButton_->Position(glm::vec3(4, 6, 18));
    whiteClockButton_->Position(glm::vec3(-4, 5.5f, 18));
  }
}
