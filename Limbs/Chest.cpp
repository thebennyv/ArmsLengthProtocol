#include "Chest.h"
#include "ChestPinout.h"
#include "UUIDs.h"
#include "ArmsLengthProtocolMessage.h"
#include "EasingFunctions.h"
#include <functional>


namespace { // anonymous

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void audioInterruptSvc()
{
    // do nothing. called while waiting for audio to end, which we don't do
}

} // namespace anonymous

Chest::Chest(BLECharacteristicRegistry& characteristicRegistry):
    characteristicRegistry(characteristicRegistry),
    msgHandler(),
    audioPlayer(),
    ARC_REACTOR_RING_NUM_PIXELS(20),
    arcReactorRingPixels(ARC_REACTOR_RING_NUM_PIXELS, (int)ChestPin_e::ArcReactorNeopixelRingPin, NEO_GRB + NEO_KHZ800),
    arcReactorCenterPixel(1, (int)ChestPin_e::ArcReactorNeopixelCenterPin, NEO_RGBW + NEO_KHZ800),
    arcReactorWhiteCycle_phase(0.0),
    arcReactorState(),
    arcReactorState_t0(),
    arcReactorState_tNextUpdate(),
    unibeamState(),
    unibeamChargePulseState(),
    unibeamChargePulseMin(),
    unibeamChargePulseMax(),
    unibeamChargePulseValue(),
    leftFlap(),
    rightFlap(),
    flapsState(),
    flapsState_t0()
{
    characteristicRegistry.attachCharacteristicRegisteredCallback(
        std::bind(
            &Chest::onCharacteristicRegistered,
            this,
            std::placeholders::_1
            )
        );
    characteristicRegistry.attachValueChangedCallback(
        std::bind(
            &Chest::onValueChanged,
            this,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4
            )
        );
}

Chest::~Chest()
{
}   

void Chest::setup()
{
    arcReactorRingPixels.begin();
    arcReactorCenterPixel.begin();
    changeArcReactorState(ArcReactorState_e::Startup);

	leftFlap.setPeriodHertz(50); // standard 50 hz servo
	leftFlap.attach((int)ChestPin_e::LeftFlapServoPin, 500, 2500);

	rightFlap.setPeriodHertz(50); // standard 50 hz servo
	rightFlap.attach((int)ChestPin_e::RightFlapServoPin, 500, 2500);

    changeFlapsState(FlapsState_e::Closing);

    audioPlayer.init(
        (int)ChestPin_e::AudioBusyPin,
        (int)ChestPin_e::AudioSerialTxPin,
        (int)ChestPin_e::AudioSerialRxPin,
        audioInterruptSvc
        );
    audioPlayer.setVolume(30); // 30 = 100% Volume
}

void Chest::loop()
{
    loopFlaps();
    loopArcReactor();
}

double Chest::scale(double norm_input, double outputMin, double outputMax)
{
    // This is a subset of the Arduino map() function, which we could have used
    // like "map(norm_angle, 0.0, 1.0, ...Min, ...Max)" except that it
    // operates on integers instead of floating point values.
    double scaled = (norm_input * (outputMax-outputMin)) + outputMin;
    return scaled;
}

void Chest::attachFlaps()
{
    if (!leftFlap.attached())
    {
        leftFlap.attach((int)ChestPin_e::LeftFlapServoPin);
    }
    if (!rightFlap.attached())
    {
        rightFlap.attach((int)ChestPin_e::RightFlapServoPin);
    }
}

void Chest::detachFlaps()
{
    if (leftFlap.attached())
    {
        leftFlap.detach();
    }
    if (rightFlap.attached())
    {
        rightFlap.detach();
    }
}

void Chest::loopFlaps()
{
    // Sample the current time once per loop to maintain consistent behavior
    // within this iteration. Two back to back calls to millis() could return
    // different values if we're at a millisecond boundary.
    const unsigned long currentTime = millis();

    // dt = delta T = change in time since we started processing a state.
    // Below, we will normalize dt to a value between 0.0 and 1.0, scaled for
    // each state's duration. 0.0 means we're at the beginnin of the state's
    // timeline, 0.5 means we're in the middle, and 1.0 means we've reached the
    // end. >1.0 means we're past the deadline.
    const unsigned long dt = currentTime - flapsState_t0;

    switch (flapsState)
    {
    case FlapsState_e::Closed:
    {
        // todo should we periodically attach and pulse the servos to ensure things stay put?
        detachFlaps();
        break;
    }
    case FlapsState_e::Opening:
    {
        const double norm_dt = (double)dt / (double)FlapsStateDurations_e::DurationOpening;
        if (norm_dt <= 1.0)
        {
            attachFlaps();

            double norm_Angle = Easing::easeInOutCubic(norm_dt);
            double angleLeft = scale(norm_Angle, (double)LeftFlapAngles_e::Min, (double)LeftFlapAngles_e::Max);
            double angleRight = scale(norm_Angle, (double)RightFlapAngles_e::Min, (double)RightFlapAngles_e::Max);
            leftFlap.write((int)angleLeft);
            rightFlap.write((int)angleRight);
        }
        if (norm_dt >= 1.0)
        {
            changeFlapsState(FlapsState_e::HoldingOpen);
        }
        break;
    }
    case FlapsState_e::HoldingOpen:
    {
        const double norm_dt = (double)dt / (double)FlapsStateDurations_e::DurationHoldingOpen;
        if (norm_dt <= 1.0)
        {
            attachFlaps();

            leftFlap.write((int)LeftFlapAngles_e::Max);
            rightFlap.write((int)RightFlapAngles_e::Max);
        }
        if (norm_dt >= 1.0)
        {
            detachFlaps(); // todo should we continue to hold the cover open while the arm opens, to ensure it doesn't accidentally retract?

            changeFlapsState(FlapsState_e::Opened);
        }
        break;
    }
    case FlapsState_e::Opened:
    {
        // todo should we periodically attach and pulse the servos to ensure things stay put?
        detachFlaps();
        break;
    }
    case FlapsState_e::Closing:
    {
        const double norm_dt = (double)dt / (double)FlapsStateDurations_e::DurationClosing;
        if (norm_dt <= 1.0)
        {
            attachFlaps();

            double norm_Angle = Easing::easeInOutCubic(1.0 - norm_dt); // 1.0-dt because we are going in reverse
            double angleLeft = scale(norm_Angle, (double)LeftFlapAngles_e::Min, (double)LeftFlapAngles_e::Max);
            double angleRight = scale(norm_Angle, (double)RightFlapAngles_e::Min, (double)RightFlapAngles_e::Max);
            leftFlap.write((int)angleLeft);
            rightFlap.write((int)angleRight);
        }
        if (norm_dt >= 1.0)
        {
            changeFlapsState(FlapsState_e::HoldingClosed);
        }
        break;
    }
    case FlapsState_e::HoldingClosed:
    {
        const double norm_dt = (double)dt / (double)FlapsStateDurations_e::DurationHoldingClosed;
        if (norm_dt <= 1.0)
        {
            attachFlaps();

            leftFlap.write((int)LeftFlapAngles_e::Min);
            rightFlap.write((int)RightFlapAngles_e::Min);
        }
        if (norm_dt >= 1.0)
        {
            detachFlaps();

            changeFlapsState(FlapsState_e::Closed);
        }
        break;
    }
    default:
        // Error
        break;
    }
}

void Chest::loopArcReactor()
{
    // Sample the current time once per loop to maintain consistent behavior
    // within this iteration. Two back to back calls to millis() could return
    // different values if we're at a millisecond boundary.
    const unsigned long currentTime = millis();

    // dt = delta T = change in time since we started processing a state.
    // Below, we will normalize dt to a value between 0.0 and 1.0, scaled for
    // each state's duration. 0.0 means we're at the beginnin of the state's
    // timeline, 0.5 means we're in the middle, and 1.0 means we've reached the
    // end. >1.0 means we're past the deadline.
    const unsigned long dt = currentTime - arcReactorState_t0;

    switch (arcReactorState)
    {
    case ArcReactorState_e::Off:
    {
        if (currentTime >= arcReactorState_tNextUpdate)
        {
            arcReactorRingPixels.clear();
            arcReactorRingPixels.setBrightness(0);
            for (int i = 0; i < arcReactorRingPixels.numPixels(); i++)
            {
                arcReactorRingPixels.setPixelColor(i, 0, 0, 0);
            }
            arcReactorRingPixels.show();

            // Every 5 seconds, ensure the pixels remain off.
            arcReactorState_tNextUpdate = currentTime + 5000; 
        }
        break;
    }
    case ArcReactorState_e::Startup:
    {
        if (dt < (int)ArcReactorStartupTimeline::_0)
        {
            // Center will remain off until the end of this sequence
            arcReactorCenterPixel.setBrightness(0); // off
            arcReactorCenterPixel.show();

            arcReactorRingPixels.setBrightness(0); // off
            arcReactorRingPixels.show();
        }
        else if (dt < (int)ArcReactorStartupTimeline::_1)
        {
            arcReactorRingPixels.setBrightness(50);
            for (int i = 0; i < arcReactorRingPixels.numPixels(); i++) {
                arcReactorRingPixels.setPixelColor(i, 115, 100, 85); // kinda dim
            }
            arcReactorRingPixels.show();
        }
        else if (dt < (int)ArcReactorStartupTimeline::_2)
        {
            arcReactorRingPixels.setBrightness(0); // off
            arcReactorRingPixels.show();
        }
        else if (dt < (int)ArcReactorStartupTimeline::_3)
        {
            arcReactorRingPixels.setBrightness(50);
            for (int i = 0; i < arcReactorRingPixels.numPixels(); i++) {
                arcReactorRingPixels.setPixelColor(i, 150, 135, 120); // a little brighter
            }
            arcReactorRingPixels.show();
        }
        else if (dt < (int)ArcReactorStartupTimeline::_4)
        {
            arcReactorRingPixels.setBrightness(0); // off
            arcReactorRingPixels.show();
        }
        else if (dt < (int)ArcReactorStartupTimeline::_5)
        {
            arcReactorRingPixels.setBrightness(50);
            for (int i = 0; i < arcReactorRingPixels.numPixels(); i++) {
                arcReactorRingPixels.setPixelColor(i, 240, 225, 210); // fairly bright
            }
            arcReactorRingPixels.show();
        }
        else if (dt < (int)ArcReactorStartupTimeline::_6)
        {
            arcReactorRingPixels.setBrightness(0); // off
            arcReactorRingPixels.show();
        }
        else if (dt < (int)ArcReactorStartupTimeline::_7)
        {
            // Center flashes on
            arcReactorCenterPixel.setBrightness(150);
            arcReactorCenterPixel.setPixelColor(0, 0, 0, 0, 255);
            arcReactorCenterPixel.show();

            // Ring ramps up around the center
            const unsigned long dt_brighten = dt - (int)ArcReactorStartupTimeline::_6;
            const unsigned long t_brighten = (int)ArcReactorStartupTimeline::_7 - (int)ArcReactorStartupTimeline::_6;
            const double norm_dt_brighten = (double)dt_brighten / (double)t_brighten;
            if (norm_dt_brighten <= 1.0)
            {
                double norm_brightness = Easing::easeInOutLinear(norm_dt_brighten);
                double scaled_brightness = scale(norm_brightness, 45, 200);

                arcReactorRingPixels.setBrightness(50);
                for (int i = 0; i < arcReactorRingPixels.numPixels(); i++) {
                    arcReactorRingPixels.setPixelColor(i, scaled_brightness, scaled_brightness, scaled_brightness); // ramp up to 200/255 brightness
                }
                arcReactorRingPixels.show();
            }
        }
        else
        {
            changeArcReactorState(ArcReactorState_e::Active);
        }

        break;
    }
    case ArcReactorState_e::Idle:
    {
        break;
    }
    case ArcReactorState_e::Active:
    {
        if (currentTime >= arcReactorState_tNextUpdate)
        {
            arcReactorCenterPixel.setBrightness(150);
            arcReactorCenterPixel.setPixelColor(0, 0, 0, 0, 255);
            arcReactorCenterPixel.show();

            // "White cycle"
            arcReactorRingPixels.setBrightness(255);
            for(int i = arcReactorRingPixels.numPixels()-1; i >= 0; i--)
            {
                int val = (int)mapf(sin(arcReactorWhiteCycle_phase + (arcReactorWhiteCycle_rate * 10 * i)), -1, 1, 200, 5);
                arcReactorRingPixels.setPixelColor(i, val, val, val);
            }
            arcReactorRingPixels.show();

            arcReactorWhiteCycle_phase -= arcReactorWhiteCycle_rate;

            // Update every 30ms
            arcReactorState_tNextUpdate = currentTime + 30; 
        }
        break;
    }
    case ArcReactorState_e::Party:
    {
        if (currentTime >= arcReactorState_tNextUpdate)
        {
            arcReactorCenterPixel.setBrightness(50);
            arcReactorCenterPixel.setPixelColor(0, 0, 0, 0, 255);
            arcReactorCenterPixel.show();

            arcReactorRingPixels.clear();
            arcReactorRingPixels.setBrightness(100);
            for (int i = 0; i < arcReactorRingPixels.numPixels(); i++)
            {
                arcReactorRingPixels.setPixelColor(i, 0, 0, 0);
            }
            arcReactorRingPixels.show();

            // Update every 10ms
            arcReactorState_tNextUpdate = currentTime + 10; 
        }
        break;
    }
    case ArcReactorState_e::UnibeamInit:
    {
        unibeamState = ArcReactorUnibeamStates_e::ChargePulse1;
        unibeamChargePulseState = ArcReactorUnibeamChargePulseStates_e::Dim;
        unibeamChargePulseMin = 50;
        unibeamChargePulseMax = 100;
        unibeamChargePulseValue = unibeamChargePulseMax;
        changeArcReactorState(ArcReactorState_e::UnibeamRun);
        break;
    }
    case ArcReactorState_e::UnibeamRun:
    {
        if (currentTime >= arcReactorState_tNextUpdate)
        {
            switch (unibeamState)
            {
            case ArcReactorUnibeamStates_e::ChargePulse1:
            case ArcReactorUnibeamStates_e::ChargePulse2:
            case ArcReactorUnibeamStates_e::ChargePulse3:
            case ArcReactorUnibeamStates_e::ChargePulse4:
            case ArcReactorUnibeamStates_e::ChargePulse5:

                arcReactorCenterPixel.setBrightness(unibeamChargePulseValue);
                arcReactorCenterPixel.setPixelColor(0, 0, 0, 0, 255);
                arcReactorCenterPixel.show();

                arcReactorRingPixels.setBrightness(unibeamChargePulseValue);
                for (int i = 0; i < arcReactorRingPixels.numPixels(); i++)
                {
                    arcReactorRingPixels.setPixelColor(i, 255, 255, 255);
                }
                arcReactorRingPixels.show();

                if (unibeamChargePulseState == ArcReactorUnibeamChargePulseStates_e::Dim)
                {
                    arcReactorState_tNextUpdate = currentTime + 3; // AKA delay(3)

                    if (--unibeamChargePulseValue <= unibeamChargePulseMin)
                    {
                        unibeamChargePulseMax += 30;
                        unibeamChargePulseState = ArcReactorUnibeamChargePulseStates_e::Brighten;
                    }
                }
                else // Brighten
                {
                    arcReactorState_tNextUpdate = currentTime + 2; // AKA delay(2)

                    if (++unibeamChargePulseValue >= unibeamChargePulseMax)
                    {
                        unibeamChargePulseMin += 30;
                        unibeamChargePulseState = ArcReactorUnibeamChargePulseStates_e::Dim;
                        ++unibeamState;
                    }
                }
                break;

            case ArcReactorUnibeamStates_e::Dim1:

                arcReactorCenterPixel.setBrightness(unibeamChargePulseValue);
                arcReactorCenterPixel.setPixelColor(0, 0, 0, 0, 255);
                arcReactorCenterPixel.show();

                arcReactorRingPixels.setBrightness(unibeamChargePulseValue);
                for (int i = 0; i < arcReactorRingPixels.numPixels(); i++)
                {
                    arcReactorRingPixels.setPixelColor(i, 255, 255, 255);
                }
                arcReactorRingPixels.show();
                
                arcReactorState_tNextUpdate = millis() + 3; // AKA delay(5)

                if (--unibeamChargePulseValue <= 20)
                {
                    unibeamState = ArcReactorUnibeamStates_e::Burst;
                }
                break;

            case ArcReactorUnibeamStates_e::Burst:

                arcReactorRingPixels.setBrightness(255);
                for (int i = 0; i < arcReactorRingPixels.numPixels(); i++)
                {
                    arcReactorRingPixels.setPixelColor(i, 255, 255, 255);
                }
                arcReactorRingPixels.show();

                arcReactorState_tNextUpdate = currentTime + 500; // Will hold burst for this long
                unibeamState = ArcReactorUnibeamStates_e::Dim2;
                unibeamChargePulseValue = 255;

                break;

            case ArcReactorUnibeamStates_e::Dim2:

                arcReactorCenterPixel.setBrightness(unibeamChargePulseValue);
                arcReactorCenterPixel.setPixelColor(0, 0, 0, 0, 255);
                arcReactorCenterPixel.show();

                arcReactorRingPixels.setBrightness(unibeamChargePulseValue);
                for (int i = 0; i < arcReactorRingPixels.numPixels(); i++)
                {
                    arcReactorRingPixels.setPixelColor(i, 255, 255, 255);
                }
                arcReactorRingPixels.show();
            
                arcReactorState_tNextUpdate = currentTime + 1; // AKA delay(1)

                if (--unibeamChargePulseValue <= 100) // magic number matching initial unibeamChargePulseMax above
                {
                    changeArcReactorState(ArcReactorState_e::Active);
                }
                break;

            default:
                // Error
                break;
            }
        }
        break;
    }
    default:
        // Error
        break;
    }
}

void Chest::changeArcReactorState(ArcReactorState_e newState)
{
    arcReactorState_t0 = millis();
    arcReactorState = newState;
    arcReactorState_tNextUpdate = 0;
}

void Chest::changeFlapsState(FlapsState_e newState)
{
    flapsState_t0 = millis();
    flapsState = newState;
}

bool Chest::onCharacteristicRegistered(BLEUUID characteristicUUID)
{
    return
        characteristicUUID.equals(UUIDs::UUID_AUDIO_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_ARC_REACTOR_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_CHEST_FLAPS_CHARACTERISTIC);
};

void Chest::onValueChanged(
    BLEUUID characteristicUUID,
    uint8_t* pData,
    size_t length,
    bool isNotify)
{
    if (characteristicUUID.equals(UUIDs::UUID_AUDIO_CHARACTERISTIC))
    {
        // Handle Audio Characteristic update
        ArmsLengthProtocolMessage msg;
        if (msgHandler.receiveUniqueMessage(msg, pData, length))
        {
            switch ((AudioCommands)msg.getCommand())
            {
            case AudioCommands::REPULSOR_CHARGE:
                audioPlayer.playFile((int)AudioFiles::RepulsorCharge);
                break;

            case AudioCommands::CLANG:
                audioPlayer.playFile((int)AudioFiles::Clang);
                break;

            case AudioCommands::BUILT_THIS_IN_A_CAVE:
                audioPlayer.playFile((int)AudioFiles::BoxOfScraps);
                break;

            case AudioCommands::REPULSOR_FIRE:
                audioPlayer.playFile((int)AudioFiles::RepulsorFire);
                break;

            case AudioCommands::UNIBEAM_CHARGE:
                audioPlayer.playFile((int)AudioFiles::UnibeamCharge);
                break;

            case AudioCommands::UNIBEAM_CHARGE_AND_FIRE:
                audioPlayer.playFile((int)AudioFiles::UnibeamChargeAndFire);
                break;

            case AudioCommands::GENIUS_BILLIONAIRE:
                audioPlayer.playFile((int)AudioFiles::GeniusBillionaire);
                break;

            default:
                // Error
                break;
            }
        }
        else
        {
            Serial.print( F("Chest::onValueChanged audio receiveUniqueMessage failed") );
        }
    }
    else if (characteristicUUID.equals(UUIDs::UUID_ARC_REACTOR_CHARACTERISTIC))
    {
        // Handle Arc Reactor Characteristic update
        ArmsLengthProtocolMessage msg;
        if (msgHandler.receiveUniqueMessage(msg, pData, length))
        {
            switch ((ArcReactorCommands)msg.getCommand())
            {
            case ArcReactorCommands::FLICKER_ON:
            //ESP.restart(); // Triggers software restart
                changeArcReactorState(ArcReactorState_e::Startup);
                break;

            case ArcReactorCommands::PARTY:
                changeArcReactorState(ArcReactorState_e::Party);
                break;

            case ArcReactorCommands::UNIBEAM:
                changeArcReactorState(ArcReactorState_e::UnibeamInit);
                break;

            default:
                // Error
                break;
            }
        }
        else
        {
            Serial.print( F("Chest::onValueChanged arc receiveUniqueMessage failed") );
        }
    }
    else if (characteristicUUID.equals(UUIDs::UUID_CHEST_FLAPS_CHARACTERISTIC))
    {
    Serial.println( F("Chest handling UUID_CHEST_FLAPS_CHARACTERISTIC") );

        // Handle Chest Flaps Characteristic update
        ArmsLengthProtocolMessage msg;
        if (msgHandler.receiveUniqueMessage(msg, pData, length))
        {
            switch ((FlapsCommands)msg.getCommand())
            {
            case FlapsCommands::CLOSE:
    Serial.println( F("Close flaps") );
                changeFlapsState(FlapsState_e::Closing);
                break;

            case FlapsCommands::OPEN:
    Serial.println( F("Open flaps") );
                changeFlapsState(FlapsState_e::Opening);
                break;

            default:
                // Error
                break;
            }
        }
        else
        {
            Serial.print( F("Chest::onValueChanged arc receiveUniqueMessage failed") );
        }
    }
}
