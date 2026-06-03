#ifndef LEFT_LEG_H
#define LEFT_LEG_H

#include "IArduinoApplication.h"
#include "ArmsLengthMessageHandler.h"
#include "BLECharacteristicRegistry.h"
#include "ESP32Servo.h"

class LeftLeg :
    public IArduinoApplication
{
public:
    LeftLeg(BLECharacteristicRegistry& characteristicRegistry);
    virtual ~LeftLeg();
    
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

    Servo flap;

    void attachFlap();
    void detachFlap();

    enum class FlapAngles_e
    {
        Min = 0, // todo
        Max = 180 // todo
    };

    // All durations are in milliseconds
    enum class StateDurations_e
    {
        DurationOpeningFlap = 1000,
        DurationHoldingFlapOpen = 250,
        DurationClosingFlap = 1000,
        DurationHoldingFlapClosed = 250
    };

    enum class FlapState_e
    {
        Closed,
        Opening,
        HoldingOpen,
        Opened,
        Closing,
        HoldingClosed
    };
    FlapState_e flapState;
    unsigned long flapState_t0;
    unsigned long flapState_tNextAction;
    void changeFlapState(
        FlapState_e newState,
        unsigned long tNextAction = 0
        );

protected:
};

#endif // LEFT_LEG_H
