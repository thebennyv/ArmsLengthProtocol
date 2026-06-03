#include "Back.h"
#include "BackPinout.h"
#include "UUIDs.h"
#include "ArmsLengthProtocolMessage.h"
#include "EasingFunctions.h"
#include <functional>

namespace { // anonymous

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double scale(double norm_input, double outputMin, double outputMax)
{
    // This is a subset of the Arduino map() function, which we could have used
    // like "map(norm_angle, 0.0, 1.0, ...Min, ...Max)" except that it
    // operates on integers instead of floating point values.
    double scaled = (norm_input * (outputMax-outputMin)) + outputMin;
    return scaled;
}

} // namespace anonymous

Back::Back(BLECharacteristicRegistry& characteristicRegistry):
    characteristicRegistry(characteristicRegistry),
    msgHandler(),
    leftShoulderFlap(),
    rightShoulderFlap(),
    leftVerticalFlap(),
    rightVerticalFlap(),
    leftHorizontalFlap(),
    rightHorizontalFlap(),
    shoulderFlapsState(),
    shoulderFlapsState_t0(),
    shoulderFlapsState_tNextAction(),
    backFlapsState(),
    backFlapsState_t0(),
    backFlapsState_tNextAction()
{
    characteristicRegistry.attachCharacteristicRegisteredCallback(
        std::bind(
            &Back::onCharacteristicRegistered,
            this,
            std::placeholders::_1
            )
        );
    characteristicRegistry.attachValueChangedCallback(
        std::bind(
            &Back::onValueChanged,
            this,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4
            )
        );
}

Back::~Back()
{
}

void Back::setup()
{
	leftShoulderFlap.setPeriodHertz(50); // standard 50 hz servo
	leftShoulderFlap.attach((int)BackPin_e::LeftShoulderFlap, 500, 2500);

	rightShoulderFlap.setPeriodHertz(50); // standard 50 hz servo
	rightShoulderFlap.attach((int)BackPin_e::RightShoulderFlap, 500, 2500);

	leftVerticalFlap.setPeriodHertz(50); // standard 50 hz servo
	leftVerticalFlap.attach((int)BackPin_e::LeftVerticalFlap, 500, 2500);

	rightVerticalFlap.setPeriodHertz(50); // standard 50 hz servo
	rightVerticalFlap.attach((int)BackPin_e::RightVerticalFlap, 500, 2500);

	leftHorizontalFlap.setPeriodHertz(50); // standard 50 hz servo
	leftHorizontalFlap.attach((int)BackPin_e::LeftHorizontalFlap, 500, 2500);

	rightHorizontalFlap.setPeriodHertz(50); // standard 50 hz servo
	rightHorizontalFlap.attach((int)BackPin_e::RightHorizontalFlap, 500, 2500);

    changeBackFlapsState(
        BackFlapsState_e::ClosingHorizontalFlaps
        );

    changeShoulderFlapsState(
        ShoulderFlapsState_e::Closing,
        // After the back flaps have (hopefully) closed
        (int)StateDurations_e::DurationClosingHorizontalFlaps +
        (int)StateDurations_e::DurationHoldingHorizontalFlapsClosed +
        (int)StateDurations_e::DurationClosingVerticalFlaps +
        (int)StateDurations_e::DurationHoldingVerticalFlapsClosed
        );
}


void Back::attachShoulderFlaps()
{
    if (!leftShoulderFlap.attached())
    {
        leftShoulderFlap.attach((int)BackPin_e::LeftShoulderFlap);
    }
    if (!rightShoulderFlap.attached())
    {
        rightShoulderFlap.attach((int)BackPin_e::RightShoulderFlap);
    }
}
void Back::attachVerticalFlaps()
{
    if (!leftVerticalFlap.attached())
    {
        leftVerticalFlap.attach((int)BackPin_e::LeftVerticalFlap);
    }
    if (!rightVerticalFlap.attached())
    {
        rightVerticalFlap.attach((int)BackPin_e::RightVerticalFlap);
    }
}
void Back::attachHorizontalFlaps()
{
    if (!leftHorizontalFlap.attached())
    {
        leftHorizontalFlap.attach((int)BackPin_e::LeftHorizontalFlap);
    }
    if (!rightHorizontalFlap.attached())
    {
        rightHorizontalFlap.attach((int)BackPin_e::RightHorizontalFlap);
    }
}
void Back::detachShoulderFlaps()
{
    if (leftShoulderFlap.attached())
    {
        leftShoulderFlap.detach();
    }
    if (rightShoulderFlap.attached())
    {
        rightShoulderFlap.detach();
    }
}
void Back::detachVerticalFlaps()
{
    if (leftVerticalFlap.attached())
    {
        leftVerticalFlap.detach();
    }
    if (rightVerticalFlap.attached())
    {
        rightVerticalFlap.detach();
    }
}
void Back::detachHorizontalFlaps()
{
    if (leftHorizontalFlap.attached())
    {
        leftHorizontalFlap.detach();
    }
    if (rightHorizontalFlap.attached())
    {
        rightHorizontalFlap.detach();
    }
}

void Back::changeShoulderFlapsState(
    ShoulderFlapsState_e newState,
    unsigned long tNextAction)
{
    shoulderFlapsState = newState;
    shoulderFlapsState_t0 = millis();
    shoulderFlapsState_tNextAction = shoulderFlapsState_t0 + tNextAction;
}

void Back::changeBackFlapsState(
    BackFlapsState_e newState,
    unsigned long tNextAction)
{
    backFlapsState = newState;
    backFlapsState_t0 = millis();
    backFlapsState_tNextAction = backFlapsState_t0 + tNextAction;
}

void Back::loop()
{
    loopShoulderFlaps();
    loopBackFlaps();
}

void Back::loopShoulderFlaps()
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
    const unsigned long dt = currentTime - shoulderFlapsState_t0;

    if (currentTime >= shoulderFlapsState_tNextAction)
    {
        switch (shoulderFlapsState)
        {
        case ShoulderFlapsState_e::Closed:
        {
            detachShoulderFlaps();
            break;
        }
        case ShoulderFlapsState_e::Opening:
        {
            attachShoulderFlaps();
            leftShoulderFlap.write((int)LeftShoulderFlapAngles_e::Max);
            rightShoulderFlap.write((int)RightShoulderFlapAngles_e::Max);
            changeShoulderFlapsState(
                ShoulderFlapsState_e::HoldingOpen,
                (int)StateDurations_e::DurationOpeningShoulderFlaps
                );
            break;
        }
        case ShoulderFlapsState_e::HoldingOpen:
        {
            attachShoulderFlaps();
            leftShoulderFlap.write((int)LeftShoulderFlapAngles_e::Max);
            rightShoulderFlap.write((int)RightShoulderFlapAngles_e::Max);
            changeShoulderFlapsState(
                ShoulderFlapsState_e::Opened,
                (int)StateDurations_e::DurationHoldingShoulderFlapsOpen
                );
            break;
        }
        case ShoulderFlapsState_e::Opened:
        {
            detachShoulderFlaps();
            break;
        }
        case ShoulderFlapsState_e::Closing:
        {
            attachShoulderFlaps();
            leftShoulderFlap.write((int)LeftShoulderFlapAngles_e::Min);
            rightShoulderFlap.write((int)RightShoulderFlapAngles_e::Min);
            changeShoulderFlapsState(
                ShoulderFlapsState_e::HoldingClosed,
                (int)StateDurations_e::DurationClosingShoulderFlaps
                );
            break;
        }
        case ShoulderFlapsState_e::HoldingClosed:
        {
            attachShoulderFlaps();
            leftShoulderFlap.write((int)LeftShoulderFlapAngles_e::Min);
            rightShoulderFlap.write((int)RightShoulderFlapAngles_e::Min);
            changeShoulderFlapsState(
                ShoulderFlapsState_e::Closed,
                (int)StateDurations_e::DurationHoldingShoulderFlapsClosed
                );
            break;
        }
        default:
            // Error
            break;
        }
    }
}

void Back::loopBackFlaps()
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
    const unsigned long dt = currentTime - backFlapsState_t0;

    if (currentTime >= backFlapsState_tNextAction)
    {
        switch (backFlapsState)
        {
        case BackFlapsState_e::Closed:
        {
            detachVerticalFlaps();
            detachHorizontalFlaps();
            break;
        }
        case BackFlapsState_e::OpeningVerticalFlaps:
        {
            attachVerticalFlaps();
            leftVerticalFlap.write((int)LeftVerticalFlapAngles_e::Max);
            rightVerticalFlap.write((int)RightVerticalFlapAngles_e::Max);
            changeBackFlapsState(
                BackFlapsState_e::HoldingVerticalFlapsOpen,
                (int)StateDurations_e::DurationOpeningVerticalFlaps
                );
            break;
        }
        case BackFlapsState_e::HoldingVerticalFlapsOpen:
        {
            attachVerticalFlaps();
            leftVerticalFlap.write((int)LeftVerticalFlapAngles_e::Max);
            rightVerticalFlap.write((int)RightVerticalFlapAngles_e::Max);
            changeBackFlapsState(
                BackFlapsState_e::OpeningHorizontalFlaps,
                (int)StateDurations_e::DurationHoldingShoulderFlapsOpen
                );
            break;
        }
        case BackFlapsState_e::OpeningHorizontalFlaps:
        {
            attachHorizontalFlaps();
            leftHorizontalFlap.write((int)LeftHorizontalFlapAngles_e::Max);
            rightHorizontalFlap.write((int)RightHorizontalFlapAngles_e::Max);
            changeBackFlapsState(
                BackFlapsState_e::HoldingHorizontalFlapsOpen,
                (int)StateDurations_e::DurationOpeningHorizontalFlaps
                );
            break;
        }
        case BackFlapsState_e::HoldingHorizontalFlapsOpen:
        {
            attachHorizontalFlaps();
            leftHorizontalFlap.write((int)LeftHorizontalFlapAngles_e::Max);
            rightHorizontalFlap.write((int)RightHorizontalFlapAngles_e::Max);
            changeBackFlapsState(
                BackFlapsState_e::Opened,
                (int)StateDurations_e::DurationOpeningHorizontalFlaps
                );
            break;
        }
        case BackFlapsState_e::Opened:
        {
            detachVerticalFlaps();
            detachHorizontalFlaps();
            break;
        }
        case BackFlapsState_e::ClosingHorizontalFlaps:
        {
            attachHorizontalFlaps();
            leftHorizontalFlap.write((int)LeftHorizontalFlapAngles_e::Min);
            rightHorizontalFlap.write((int)RightHorizontalFlapAngles_e::Min);
            changeBackFlapsState(
                BackFlapsState_e::HoldingHorizontalFlapsClosed,
                (int)StateDurations_e::DurationClosingHorizontalFlaps
                );
            break;
        }
        case BackFlapsState_e::HoldingHorizontalFlapsClosed:
        {
            attachHorizontalFlaps();
            leftHorizontalFlap.write((int)LeftHorizontalFlapAngles_e::Min);
            rightHorizontalFlap.write((int)RightHorizontalFlapAngles_e::Min);
            changeBackFlapsState(
                BackFlapsState_e::ClosingVerticalFlaps,
                (int)StateDurations_e::DurationHoldingHorizontalFlapsClosed
                );
            break;
        }
        case BackFlapsState_e::ClosingVerticalFlaps:
        {
            attachVerticalFlaps();
            leftVerticalFlap.write((int)LeftVerticalFlapAngles_e::Min);
            rightVerticalFlap.write((int)RightVerticalFlapAngles_e::Min);
            changeBackFlapsState(
                BackFlapsState_e::HoldingVerticalFlapsClosed,
                (int)StateDurations_e::DurationClosingVerticalFlaps
                );
            break;
        }
        case BackFlapsState_e::HoldingVerticalFlapsClosed:
        {
            attachVerticalFlaps();
            leftVerticalFlap.write((int)LeftVerticalFlapAngles_e::Min);
            rightVerticalFlap.write((int)RightVerticalFlapAngles_e::Min);
            changeBackFlapsState(
                BackFlapsState_e::Closed,
                (int)StateDurations_e::DurationHoldingShoulderFlapsClosed
                );
            break;
        }
        default:
            // Error
            break;
        }
    }
}

bool Back::onCharacteristicRegistered(BLEUUID characteristicUUID)
{
    return characteristicUUID.equals(UUIDs::UUID_BACK_FLAPS_CHARACTERISTIC) ||
           characteristicUUID.equals(UUIDs::UUID_SHOULDER_FLAPS_CHARACTERISTIC);
};

void Back::onValueChanged(
    BLEUUID characteristicUUID,
    uint8_t* pData,
    size_t length,
    bool isNotify)
{
    if (characteristicUUID.equals(UUIDs::UUID_BACK_FLAPS_CHARACTERISTIC))
    {
    Serial.println( F("Back handling UUID_BACK_FLAPS_CHARACTERISTIC") );

        // Handle Back Flaps Characteristic update
        ArmsLengthProtocolMessage msg;
        if (msgHandler.receiveUniqueMessage(msg, pData, length))
        {
            switch ((FlapsCommands)msg.getCommand())
            {
            case FlapsCommands::CLOSE:
    Serial.println( F("Back close back flaps") );
                changeBackFlapsState(
                    BackFlapsState_e::ClosingHorizontalFlaps
                    );
                changeShoulderFlapsState(
                    ShoulderFlapsState_e::Closing,
                    // After the back flaps have (hopefully) closed
                    (int)StateDurations_e::DurationClosingHorizontalFlaps +
                    (int)StateDurations_e::DurationHoldingHorizontalFlapsClosed +
                    (int)StateDurations_e::DurationClosingVerticalFlaps +
                    (int)StateDurations_e::DurationHoldingVerticalFlapsClosed
                    );
                break;

            case FlapsCommands::OPEN:
    Serial.println( F("Back open back flaps") );
                changeShoulderFlapsState(
                    ShoulderFlapsState_e::Opening
                    );
                changeBackFlapsState(
                    BackFlapsState_e::OpeningVerticalFlaps,
                    // After the shoulder flaps have (hopefully) opened
                    (int)StateDurations_e::DurationOpeningShoulderFlaps +
                    (int)StateDurations_e::DurationHoldingShoulderFlapsOpen
                    );
                break;

            default:
    Serial.println( F("Back unknown back flaps command") );
                // Error
                break;
            }
        }
        else
        {
            Serial.print( F("Back::onValueChanged back flaps receiveUniqueMessage failed") );
        }
    }
    else if (characteristicUUID.equals(UUIDs::UUID_SHOULDER_FLAPS_CHARACTERISTIC))
    {
    Serial.println( F("Back handling UUID_SHOULDER_FLAPS_CHARACTERISTIC") );

        // Handle Shoulder Flaps Characteristic update
        ArmsLengthProtocolMessage msg;
        if (msgHandler.receiveUniqueMessage(msg, pData, length))
        {
            switch ((FlapsCommands)msg.getCommand())
            {
            case FlapsCommands::CLOSE:
                if (backFlapsState == BackFlapsState_e::Closed
                 || backFlapsState == BackFlapsState_e::HoldingVerticalFlapsClosed)
                {
                    changeShoulderFlapsState(ShoulderFlapsState_e::Closing);
                }
                else
                {
                    // Force the back flaps to close first (unsafe to close shoulder flaps while vertical flaps are open)
                    changeBackFlapsState(
                        BackFlapsState_e::ClosingHorizontalFlaps
                        );
                    changeShoulderFlapsState(
                        ShoulderFlapsState_e::Closing,
                        // After the back flaps have (hopefully) closed
                        (int)StateDurations_e::DurationClosingHorizontalFlaps +
                        (int)StateDurations_e::DurationHoldingHorizontalFlapsClosed +
                        (int)StateDurations_e::DurationClosingVerticalFlaps +
                        (int)StateDurations_e::DurationHoldingVerticalFlapsClosed
                        );
                }
                break;

            case FlapsCommands::OPEN:
    Serial.println( F("Back open shoulder flaps") );
                changeShoulderFlapsState(ShoulderFlapsState_e::Opening);
                break;

            default:
    Serial.println( F("Back unknown shoulder flaps command") );
                // Error
                break;
            }
        }
        else
        {
            Serial.print( F("Back::onValueChanged shoulder flaps receiveUniqueMessage failed") );
        }
    }
}
