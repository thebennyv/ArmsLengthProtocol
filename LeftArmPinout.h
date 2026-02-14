#ifndef LEFTARMPINOUT_H
#define LEFTARMPINOUT_H

#include <Arduino.h>

enum class LeftArmPin_e
{
    ThumbButton = D0,
    IndexFingerButton = D1,
    MiddleFingerButton = D2,
    RingFingerButton = D3,
    PinkyFingerButton = D4,

    // D5
    // D6 -- output only due to Serial?

    RocketCoverServo = D7,
    RocketArmServo = D8

    // D9
    // D10
};

#endif // LEFTARMPINOUT_H
