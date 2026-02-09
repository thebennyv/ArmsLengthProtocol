#ifndef RIGHT_ARM_H
#define RIGHT_ARM_H

#include "IArduinoApplication.h"
#include "ArmsLengthMessageHandler.h"
#include "BLECharacteristicRegistry.h"
#include "ESP32Servo.h"

class RightArm :
    public IArduinoApplication
{
public:
    RightArm(BLECharacteristicRegistry& characteristicRegistry);
    virtual ~RightArm();
    
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

    void loopLaser();
    void loopRocket();
    void loopRepulsor();

protected:
};

#endif // RIGHT_ARM_H
