#ifndef HELMETPINOUT_H
#define HELMETPINOUT_H

#include <Arduino.h>

enum class HelmetPin_e
{
    FacePlateEyes = D8,
    
    // D1
    // D2
    // D3
    // D4
    // AudioPlayerBusy = D5,
    // AudioPlayerTx = D2,
    // AudioPlayerRx = D3,

    FaceplateServo1 = D1,
    FaceplateServo2 = D2

    // D9
    // D10
};

#endif // HELMETPINOUT_H
