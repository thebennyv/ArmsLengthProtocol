#ifndef HELMET_H
#define HELMET_H

#include "IArduinoApplication.h"
#include "ArmsLengthMessageHandler.h"
#include "BLECharacteristicRegistry.h"
#include "ESP32Servo.h"

class Helmet :
    public IArduinoApplication
{
public:
    Helmet(BLECharacteristicRegistry& characteristicRegistry);
    virtual ~Helmet();
    
    void setup() override;
    void loop() override;

    bool onCharacteristicRegistered(BLEUUID characteristicUUID);

    void onValueChanged(
        BLEUUID characteristicUUID,
        uint8_t* pData,
        size_t length,
        bool isNotify
        );

private:

    BLECharacteristicRegistry& characteristicRegistry;

    ArmsLengthMessageHandler msgHandler;

    Servo faceplateServo1;
    Servo faceplateServo2;
    const int faceplateServo1Pin;
    const int faceplateServo2Pin;
    void attachFaceplateServos();
    void detachFaceplateServos();
    enum class FaceplateServoAngles_e
    {
        servo1Min = 20, // todo polarity and iron man servo says 20-160
        servo1Max = 160, // todo polarity
        servo2Min = 160, // todo polarity
        servo2Max = 20 // todo polarity
    };
    double scaleFaceplateServo1Angle(double norm_angle /* 0.0 to 1.0 */);
    double scaleFaceplateServo2Angle(double norm_angle /* 0.0 to 1.0 */);

    // All durations are in milliseconds
    enum class FaceplateStateDurations_e
    {
        
        DurationOpening       = 1000, // 1.0 seconds to open
        DurationHoldingOpen   =  250, // then briefly hold open
        
        DurationClosing       = 1000, // 1.0 seconds to close
        DurationHoldingClosed =  250  // then briefly hold closed
    };

    enum class FaceplateState_e
    {
        Closed,
        Opening,
        HoldingOpen,
        Opened,
        Closing,
        HoldingClosed
    };
    FaceplateState_e faceplateState;
    unsigned long faceplateState_t0;
    void changeFaceplateState(FaceplateState_e newState);

    void loopFaceplate();
    void loopEyes();

protected:
};

#endif // HELMET_H
