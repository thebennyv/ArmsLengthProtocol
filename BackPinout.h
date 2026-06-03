#ifndef BACKPINOUT_H
#define BACKPINOUT_H

#include <Arduino.h>

enum class BackPin_e
{
    LeftShoulderFlap = D0,
    RightShoulderFlap = D1,

    LeftVerticalFlap = D2,
    RightVerticalFlap = D3,

    LeftHorizontalFlap = D4,
    RightHorizontalFlap = D5
};

#endif // BACKPINOUT_H
