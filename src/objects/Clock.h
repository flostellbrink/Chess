#ifndef CLOCK_H
#define CLOCK_H

#include "BasicObject.h"
#include "Updatable.h"

class Clock : public Updatable
{
public:
  Clock();
  void Update(float elapsedTimeMs) override;

protected:
  BasicObject white_minutes_hand_, white_seconds_hand_, white_clock_button_;
  BasicObject black_minutes_hand_, black_seconds_hand_, black_clock_button_;

  float GetHandRotation(float time, int secondsPerRotation) const;

private:
  float time_counter_ = 0.f, delay_ = 7000.f; // after DELAY ms, reduce time from the clock
  float white_time_, black_time_, start_time_ = 300000.f; // time for the players
  bool white_on_clock_ = true;
};


#endif // CLOCK_H
