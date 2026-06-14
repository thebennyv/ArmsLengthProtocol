#ifndef LEFT_ARM_H
#define LEFT_ARM_H

#include "IArduinoApplication.h"
#include "ArmsLengthMessageHandler.h"
#include "BLECharacteristicRegistry.h"
#include "lib/ESP32Servo.h"
#include <Adafruit_NeoPixel.h>

class LeftArm :
    public IArduinoApplication
{
public:
    LeftArm(BLECharacteristicRegistry& characteristicRegistry);
    virtual ~LeftArm();
    
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

    Servo rocketCover;
    const int rocketCoverServoPin;
    void attachRocketCover();
    void detachRocketCover();
    enum class RocketCoverAngles_e
    {
        Min = 0, // todo
        Max = 180 // todo
    };
    double scaleRocketCoverAngle(double norm_coverAngle /* 0.0 to 1.0 */);

    Servo rocketArm;
    const int rocketArmServoPin;
    void attachRocketArm();
    void detachRocketArm();
    enum class RocketArmAngles_e
    {
        Min = 0, // todo
        Max = 180 // todo
    };
    double scaleRocketArmAngle(double norm_armAngle /* 0.0 to 1.0 */);

    Servo laserMotor;
    const int laserMotorPin;
    void attachLaserMotor();
    void detachLaserMotor();
    enum class LaserMotorAngles_e
    {
        Min = 0, // todo
        Max = 180 // todo
    };
    double scaleLaserMotorAngle(double norm_motorAngle /* 0.0 to 1.0 */);

    // All durations are in milliseconds
    enum class RocketStateDurations_e
    {
        // 2.25 seconds to open
        DurationOpeningCover = 1000,
        DurationHoldingCoverOpen = 250,
        DurationOpeningRocket = 1000,
        // then briefly hold open
        DurationHoldingRocketOpen = 250,
        
        // 2.25 seconds to close
        DurationClosingRocket = 1000,
        DurationHoldingRocketClosed = 250,
        DurationClosingCover = 1000,
        // then briefly hold closed
        DurationHoldingCoverClosed = 250
    };

    enum class RocketState_e
    {
        Closed,
        OpeningCover,
        HoldingCoverOpen,
        OpeningRocket,
        HoldingRocketOpen,
        Opened,
        ClosingRocket,
        HoldingRocketClosed,
        ClosingCover,
        HoldingCoverClosed
    };
    RocketState_e rocketState;
    unsigned long rocketState_t0;
    void changeRocketState(RocketState_e newState);

    // All durations are in milliseconds
    enum class LaserStateDurations_e
    {
        DurationOpening = 220,
        DurationClosing = 150
    };

    enum class LaserState_e
    {
        Off,
        Opening,
        On,
        Closing
    };
    LaserState_e laserState;
    unsigned long laserState_t0;
    void changeLaserState(LaserState_e newState);

    Adafruit_NeoPixel repulsorLED;

    void loopLaser();
    void loopRocket();
    void loopRepulsor();

protected:
};

#endif // LEFT_ARM_H
