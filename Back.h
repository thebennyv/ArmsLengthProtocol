#ifndef BACK_H
#define BACK_H

#include "IArduinoApplication.h"
#include "ArmsLengthMessageHandler.h"
#include "BLECharacteristicRegistry.h"
#include "ESP32Servo.h"

class Back :
    public IArduinoApplication
{
public:
    Back(BLECharacteristicRegistry& characteristicRegistry);
    virtual ~Back();
    
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

    Servo leftShoulderFlap;
    Servo rightShoulderFlap;
    Servo leftVerticalFlap;
    Servo rightVerticalFlap;
    Servo leftHorizontalFlap;
    Servo rightHorizontalFlap;

    void attachShoulderFlaps();
    void attachVerticalFlaps();
    void attachHorizontalFlaps();
    void detachShoulderFlaps();
    void detachVerticalFlaps();
    void detachHorizontalFlaps();

    enum class LeftShoulderFlapAngles_e
    {
        Min = 180-80,
        Max = 180
    };

    enum class RightShoulderFlapAngles_e
    {
        Min = 180,
        Max = 180-80 // todo adjust with new arm
    };








enum {adjustVericals = 70};

    enum class LeftVerticalFlapAngles_e
    {
        Min = 180-130, // 50-90
        Max = 180-90+50 // open
    };

    enum class RightVerticalFlapAngles_e
    {
        Min = 0+140, // 140-100
        Max = 0+100-50 //  open
    };




    enum class LeftHorizontalFlapAngles_e
    {
        Min = 80, // todo
        Max = 110 // todo
    };

    enum class RightHorizontalFlapAngles_e
    {
        Min = 80, // todo
        Max = 110 // todo
    };

    // All durations are in milliseconds
    enum class StateDurations_e
    {
        DurationOpeningShoulderFlaps = 500,
        DurationHoldingShoulderFlapsOpen = 150,
        DurationClosingShoulderFlaps = 500,
        DurationHoldingShoulderFlapsClosed = 150,

        DurationOpeningVerticalFlaps = 1000,
        DurationHoldingVerticalFlapsOpen = 250,
        DurationOpeningHorizontalFlaps = 1000,
        DurationHoldingHorizontalFlapsOpen = 250,
        DurationClosingHorizontalFlaps = 1000,
        DurationHoldingHorizontalFlapsClosed = 250,
        DurationClosingVerticalFlaps = 1000,
        DurationHoldingVerticalFlapsClosed = 250
    };

    enum class ShoulderFlapsState_e
    {
        Closed,
        Opening,
        HoldingOpen,
        Opened,
        Closing,
        HoldingClosed
    };
    ShoulderFlapsState_e shoulderFlapsState;
    unsigned long shoulderFlapsState_t0;
    unsigned long shoulderFlapsState_tNextAction;
    void changeShoulderFlapsState(
        ShoulderFlapsState_e newState,
        unsigned long tNextAction = 0
        );

    enum class BackFlapsState_e
    {
        Closed,
        OpeningVerticalFlaps,
        HoldingVerticalFlapsOpen,
        OpeningHorizontalFlaps,
        HoldingHorizontalFlapsOpen,
        Opened,
        ClosingHorizontalFlaps,
        HoldingHorizontalFlapsClosed,
        ClosingVerticalFlaps,
        HoldingVerticalFlapsClosed
    };
    BackFlapsState_e backFlapsState;
    unsigned long backFlapsState_t0;
    unsigned long backFlapsState_tNextAction;
    void changeBackFlapsState(
        BackFlapsState_e newState,
        unsigned long tNextAction = 0
        );

    void loopShoulderFlaps();
    void loopBackFlaps();

protected:
};

#endif // BACK_H
