#ifndef RIGHTARMPINOUT_H
#define RIGHTARMPINOUT_H

#include <Arduino.h>

enum class RightArmPin_e
{
    // D6 -- output only due to Serial?
    
    ThumbButton = D0,
    IndexFingerButton = D1,
    MiddleFingerButton = D2,
    RingFingerButton = D3,
    RocketCoverServo = D4,
    RocketArmServo = D5,
    LaserMotor = D6,
	// D7
    Laser = D8,
    RepulsorNeoPixel = D9,
    FlexSensor = A10
};

#endif // RIGHTARMPINOUT_H
