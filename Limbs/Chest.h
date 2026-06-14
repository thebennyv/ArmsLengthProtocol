#ifndef CHEST_H
#define CHEST_H

#include "IArduinoApplication.h"
#include "ArmsLengthMessageHandler.h"
#include "BLECharacteristicRegistry.h"
#include "lib/ESP32Servo.h"
#include <Adafruit_NeoPixel.h>
#include "DFPlayerMini.h"

class Chest :
    public IArduinoApplication
{
public:
    Chest(BLECharacteristicRegistry& characteristicRegistry);
    virtual ~Chest();
    
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

    DFPlayerMini audioPlayer;

    const int ARC_REACTOR_RING_NUM_PIXELS;
    Adafruit_NeoPixel arcReactorRingPixels;
    Adafruit_NeoPixel arcReactorCenterPixel;

    float arcReactorWhiteCycle_phase; // radians (2Pi per cycle)
    const float arcReactorWhiteCycle_rate = 0.05; // radians

    enum class ArcReactorState_e
    {
        Off,
        Startup,
        Idle,
        Active,
        Party,
        UnibeamInit,
        UnibeamRun
    };
    ArcReactorState_e arcReactorState;
    unsigned long arcReactorState_t0;
    unsigned long arcReactorState_tNextUpdate;
    void changeArcReactorState(ArcReactorState_e newState);

    enum class ArcReactorStartupTimeline
    {
        _0 =      1500, // off
        _1 = _0 +   50, // kinda dim
        _2 = _1 +  100, // off
        _3 = _2 +   50, // a little brighter
        _4 = _3 +  100, // off
        _5 = _4 +   50, // fairly bright
        _6 = _5 +  500, // off
        _7 = _6 + 1500  // ramp up to 200/255 brightness
    };

    enum ArcReactorUnibeamStates_e
    {
        ChargePulse1,
        ChargePulse2,
        ChargePulse3,
        ChargePulse4,
        ChargePulse5,
        Dim1,
        Burst,
        Dim2
    };
    /*ArcReactorUnibeamStates_e*/ int unibeamState;

    // inline ArcReactorUnibeamStates_e& operator++(ArcReactorUnibeamStates_e& v)
    // {
    //     using Underlying = std::underlying_type_t<ArcReactorUnibeamStates_e>;
    //     v = static_cast<ArcReactorUnibeamStates_e>(static_cast<Underlying>(v) + 1);
    //     return v;
    // }

    // inline ArcReactorUnibeamStates_e operator++(ArcReactorUnibeamStates_e& v, int)
    // {
    //     ArcReactorUnibeamStates_e result = v;
    //     ++v; // Calls the prefix operator above
    //     return result;
    // }

    enum class ArcReactorUnibeamChargePulseStates_e
    {
        Dim,
        Brighten
    };
    ArcReactorUnibeamChargePulseStates_e unibeamChargePulseState;
    int unibeamChargePulseMin;
    int unibeamChargePulseMax;
    int unibeamChargePulseValue;

    enum class AudioFiles
    {
        RepulsorCharge = 1,
        Clang = 2,
        BoxOfScraps = 3,
        RepulsorFire = 4,
        RepulsorMultiFire = 5,
        UnibeamCharge = 6,
        UnibeamChargeAndFire = 7,
        GeniusBillionaire = 8
    };


    
    Servo leftFlap;
    Servo rightFlap;
    void attachFlaps();
    void detachFlaps();
    enum class LeftFlapAngles_e
    {
        Min = 90, // todo
        Max = 10 // todo
    };
    enum class RightFlapAngles_e
    {
        Min = 10, // todo
        Max = 90 // todo
    };

    // All durations are in milliseconds
    enum class FlapsStateDurations_e
    {
        DurationOpening = 1000,
        DurationHoldingOpen = 250,
        
        DurationClosing = 1000,
        DurationHoldingClosed = 250
    };

    enum class FlapsState_e
    {
        Closed,
        Opening,
        HoldingOpen,
        Opened,
        Closing,
        HoldingClosed
    };
    FlapsState_e flapsState;
    unsigned long flapsState_t0;
    void changeFlapsState(FlapsState_e newState);

    double scale(double norm_input, double outputMin, double outputMax);

    void loopArcReactor();
    void loopFlaps();

protected:
};

#endif // CHEST_H
