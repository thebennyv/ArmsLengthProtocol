#ifndef CHESTPINOUT_H
#define CHESTPINOUT_H

#include <Arduino.h>

enum class ChestPin_e
{
    AudioBusyPin = D3, // not connected
    AudioSerialRxPin = D4,
    AudioSerialTxPin = D5,
    RightFlapServoPin = D2,
    LeftFlapServoPin = D3,
    // D4
    // D5
    // D6 -- output only due to Serial?
    // D7
    ArcReactorNeopixelRingPin = D8,
    ArcReactorNeopixelCenterPin = D9
    // D10
};

#endif // CHESTPINOUT_H
