#ifndef CLOCK_H
#define CLOCK_H

class BasicObject;

class Clock {
public:
  Clock();
  void Update(float elapsedTimeMs, bool whiteTurn);
  void SwitchPlayer(bool whiteTurn);
  int Timeout() const;

protected:
  BasicObject* whiteMinutesHand_;
  BasicObject* whiteSecondsHand_;
  BasicObject* whiteClockButton_;

  BasicObject* blackMinutesHand_;
  BasicObject* blackSecondsHand_;
  BasicObject* blackClockButton_;

  float getHandRotation(float time, int secondsPerRotation) const;

private:
  float timeCounter = 0.f, delay = 7000.f; // after DELAY ms, reduce time from the clock
  float whiteTime, blackTime, startTime = 300000.f; // time for the players
  bool whiteOnClock_ = true;
  int timeoutStatus_ = 0; // No player is timed out
};


#endif // CLOCK_H
