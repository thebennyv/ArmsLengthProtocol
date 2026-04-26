#ifndef HELMETPINOUT_H
#define HELMETPINOUT_H

#include <Arduino.h>

enum class HelmetPin_e
{
    // D0
    // D1
    // D2
    // D3
    // D4
    // D5
    // D6 -- output only due to Serial?

    FaceplateServo1 = D7,
    FaceplateServo2 = D8

    // D9
    // D10
};

#endif // HELMETPINOUT_H
