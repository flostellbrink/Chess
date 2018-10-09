#include <GL/glew.h>
#include "clock.hpp"
#include <string>
#include "math.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <src/geometry/geoManager.hpp>
#include <iostream>
#include "src/objects.hpp"
#include "objectManager.hpp"
#include "moves/moveBase.hpp"
#include "moves/move.hpp"
#include "moves/transform.hpp"

using std::string;
using glm::vec3;

const int boardSize = 8;

Clock::Clock() {
    whiteTime = blackTime = startTime;
    ObjectManager* mngr = &ObjectManager::Instance;

    /* Clock case */
    mngr->AddObject(new BasicObject(objects::ClockBase, vec3(0,-1,18)));

    mngr->AddObject(new BasicObject(objects::ClockLeg, vec3( 4,-1,18.5)));
    mngr->AddObject(new BasicObject(objects::ClockLeg, vec3(-4,-1,18.5)));

    mngr->AddObject(new BasicObject(objects::ClockLeg, vec3( 4,-1,17.5)));
    mngr->AddObject(new BasicObject(objects::ClockLeg, vec3(-4,-1,17.5)));


    /* Clock parts for BLACK */
    _blackClockButton = new BasicObject(objects::ClockButton, vec3(4, 5.5, 18));
    mngr->AddObject(_blackClockButton);
    mngr->AddObject(new BasicObject(objects::ClockButtonBorder, vec3(4,6,18))); // socket of the clock button

    BasicObject* tmp = new BasicObject(objects::ClockGlassBorder, vec3(3.4, 3, 17));
    tmp->setRotationX(-1.5708f);
    mngr->AddObject(tmp); // frame of the clock-face

    tmp = new BasicObject(objects::ClockFace, vec3(3.4, 3, 16.9), 0, "noLight");
    tmp->setRotationX(-1.5708f);
    mngr->AddObject(tmp); // clock face

    // clock hands
    _blackMinutesHand = new BasicObject(objects::blackMinutesHand, vec3(3.4, 3, 16.8), 0, "noLight");
    mngr->AddObject(_blackMinutesHand);

    _blackSecondsHand = new BasicObject(objects::blackSecondsHand, vec3(3.4, 3, 16.8), 0, "noLight");
    mngr->AddObject(_blackSecondsHand);

    // Time cap indicator
    tmp = new BasicObject(objects::ClockTimeCap, vec3(3.4, 3, 16.8), 0, "noLight");
    tmp->setRotationZ( 2*glm::pi<float>() * (blackTime / 3600000) );
    mngr->AddObject(tmp);

    /* Clock parts for WHITE */
    _whiteClockButton = new BasicObject(objects::ClockButton, vec3(-4, 6, 18));
    mngr->AddObject(_whiteClockButton);
    mngr->AddObject(new BasicObject(objects::ClockButtonBorder, vec3(-4,6,18))); // socket of the clock button

    tmp = new BasicObject(objects::ClockGlassBorder, vec3(-3.4, 3, 17));
    tmp->setRotationX(-1.5708f);
    mngr->AddObject(tmp); // frame of the clock-face

    tmp = new BasicObject(objects::ClockFace, vec3(-3.4, 3, 16.9), 0, "noLight");
    tmp->setRotationX(-1.5708f);
    mngr->AddObject(tmp); // clock face

    // clock hands
    _whiteMinutesHand = new BasicObject(objects::whiteMinutesHand, vec3(-3.4, 3, 16.8), 0, "noLight");
    mngr->AddObject(_whiteMinutesHand);

    _whiteSecondsHand = new BasicObject(objects::whiteSecondsHand, vec3(-3.4, 3, 16.8), 0, "noLight");
    mngr->AddObject(_whiteSecondsHand);

    // Time cap indicator
    tmp = new BasicObject(objects::ClockTimeCap, vec3(-3.4, 3, 16.8), 0, "noLight");
    tmp->setRotationZ( 2*glm::pi<float>() * (whiteTime / 3600000) );
    mngr->AddObject(tmp);
}

float Clock::getHandRotation(float time, int secondsPerRotation){
    float msPerRotation = (float)(secondsPerRotation * 1000);
    return -2 * glm::pi<float>() * (time - startTime) / msPerRotation;
}

void Clock::Update(float elapsedTimeMs, bool whiteTurn) {
    timeCounter += elapsedTimeMs;

    if (whiteTurn != _whiteOnClock) {
        _whiteOnClock = whiteTurn;
        SwitchPlayer(whiteTurn);
    }

    // Delay cap is hit, start reducing time from the active player's clock
    if (timeCounter >= delay) {
        if (_whiteOnClock) { // It's white's turn..
            if (whiteTime > 0) { // White still has time remaining, reduce time and move clock hands
                whiteTime-=elapsedTimeMs;

                _whiteSecondsHand->setRotationZ(getHandRotation(whiteTime, 60));
                _whiteMinutesHand->setRotationZ(getHandRotation(whiteTime, 3600));
            } else              // White has no more time, set timeout status indicating black wins
               _timeoutStatus = 3;

        } else {
            if (blackTime > 0) {
                blackTime-=elapsedTimeMs;

                _blackSecondsHand->setRotationZ(getHandRotation(blackTime, 60));
                _blackMinutesHand->setRotationZ(getHandRotation(blackTime, 3600));
            } else
               _timeoutStatus = 2;
        }
//        std::cout << "ChessInfo: Delay overrun " << (_whiteOnClock ? whiteTime : blackTime) << std::endl;
    }
}

int Clock::timeout() {
    return _timeoutStatus;
}

void Clock::SwitchPlayer(bool whiteTurn) {
    timeCounter = 0.f;

    if (whiteTurn) {
        _blackClockButton->Position(vec3(4, 5.5, 18));
        _whiteClockButton->Position(vec3(-4, 6, 18));
    } else {
        _blackClockButton->Position(vec3(4, 6, 18));
        _whiteClockButton->Position(vec3(-4, 5.5, 18));

    }
}
