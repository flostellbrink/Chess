#ifndef CLOCK_H
#define CLOCK_H

#include <GL/glew.h>
#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "drawable.hpp"
#include <vector>
#include <random>
#include <stack>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "geometry/geometry.hpp"
#include "geometry/geoManager.hpp"

using std::string;
using std::vector;
using std::stack;

class BasicObject;

class Clock {
public:
    Clock();
    void Update(float elapsedTimeMs, bool whiteTurn);
    void SwitchPlayer(bool whiteTurn);
    int timeout();

protected:
    BasicObject* _whiteMinutesHand;
    BasicObject* _whiteSecondsHand;
    BasicObject* _whiteClockButton;

    BasicObject* _blackMinutesHand;
    BasicObject* _blackSecondsHand;
    BasicObject* _blackClockButton;

    float getHandRotation(float time, int secondsPerRotation);

private:
    float timeCounter = 0.f, delay = 7000.f; // after DELAY ms, reduce time from the clock
    float whiteTime, blackTime, startTime = 300000.f; // time for the players
    bool _whiteOnClock = true;
    int _timeoutStatus = 0; // No player is timed out
};


#endif // CLOCK_H
