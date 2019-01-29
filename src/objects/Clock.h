#ifndef CLOCK_H
#define CLOCK_H

class BasicObject;

class Clock
{
public:
  Clock();
  void Update(float elapsedTimeMs, bool whiteTurn);
  void SwitchPlayer(bool whiteTurn);
  int Timeout() const;

protected:
  BasicObject* white_minutes_hand_;
  BasicObject* white_seconds_hand_;
  BasicObject* white_clock_button_;

  BasicObject* black_minutes_hand_;
  BasicObject* black_seconds_hand_;
  BasicObject* black_clock_button_;

  float GetHandRotation(float time, int secondsPerRotation) const;

private:
  float time_counter_ = 0.f, delay_ = 7000.f; // after DELAY ms, reduce time from the clock
  float white_time_, black_time_, start_time_ = 300000.f; // time for the players
  bool white_on_clock_ = true;
  int timeout_status_ = 0; // No player is timed out
};


#endif // CLOCK_H
