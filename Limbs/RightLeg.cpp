#include "RightLeg.h"
#include "RightLegPinout.h"
#include "UUIDs.h"
#include "ArmsLengthProtocolMessage.h"
#include "EasingFunctions.h"
#include <functional>

RightLeg::RightLeg(BLECharacteristicRegistry& characteristicRegistry):
    characteristicRegistry(characteristicRegistry),
    msgHandler(),
    flap(),
    flapState(),
    flapState_t0(),
    flapState_tNextAction()
{
    characteristicRegistry.attachCharacteristicRegisteredCallback(
        std::bind(
            &RightLeg::onCharacteristicRegistered,
            this,
            std::placeholders::_1
            )
        );
    characteristicRegistry.attachValueChangedCallback(
        std::bind(
            &RightLeg::onValueChanged,
            this,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4
            )
        );
}

RightLeg::~RightLeg()
{
}

void RightLeg::setup()
{
	flap.setPeriodHertz(50); // standard 50 hz servo
	flap.attach((int)RightLegPin_e::Flap, 500, 2500);

    changeFlapState(
        FlapState_e::Closing
        );
}

void RightLeg::loop()
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
    const unsigned long dt = currentTime - flapState_t0;

    if (currentTime >= flapState_tNextAction)
    {
        switch (flapState)
        {
        case FlapState_e::Closed:
        {
            detachFlap();
            break;
        }
        case FlapState_e::Opening:
        {
            attachFlap();
            flap.write((int)FlapAngles_e::Max);
            changeFlapState(
                FlapState_e::HoldingOpen,
                (int)StateDurations_e::DurationOpeningFlap
                );
            break;
        }
        case FlapState_e::HoldingOpen:
        {
            attachFlap();
            flap.write((int)FlapAngles_e::Max);
            changeFlapState(
                FlapState_e::Opened,
                (int)StateDurations_e::DurationHoldingFlapOpen
                );
            break;
        }
        case FlapState_e::Opened:
        {
            detachFlap();
            break;
        }
        case FlapState_e::Closing:
        {
            attachFlap();
            flap.write((int)FlapAngles_e::Min);
            changeFlapState(
                FlapState_e::HoldingClosed,
                (int)StateDurations_e::DurationClosingFlap
                );
            break;
        }
        case FlapState_e::HoldingClosed:
        {
            attachFlap();
            flap.write((int)FlapAngles_e::Min);
            changeFlapState(
                FlapState_e::Closed,
                (int)StateDurations_e::DurationHoldingFlapClosed
                );
            break;
        }
        default:
            // Error
            break;
        }
    }
}

void RightLeg::attachFlap()
{
    if (!flap.attached())
    {
        flap.attach((int)RightLegPin_e::Flap);
    }
}

void RightLeg::detachFlap()
{
    if (flap.attached())
    {
        flap.detach();
    }
}

void RightLeg::changeFlapState(
    FlapState_e newState,
    unsigned long tNextAction)
{
    flapState = newState;
    flapState_t0 = millis();
    flapState_tNextAction = flapState_t0 + tNextAction;
}

bool RightLeg::onCharacteristicRegistered(BLEUUID characteristicUUID)
{
    return characteristicUUID.equals(UUIDs::UUID_RIGHT_LEG_FLAPS_CHARACTERISTIC);
};

void RightLeg::onValueChanged(
    BLEUUID characteristicUUID,
    uint8_t* pData,
    size_t length,
    bool isNotify)
{
    if (characteristicUUID.equals(UUIDs::UUID_RIGHT_LEG_FLAPS_CHARACTERISTIC))
    {
    Serial.println( F("RightLeg handling UUID_RIGHT_LEG_FLAPS_CHARACTERISTIC") );

        // Handle Right Leg Flaps Characteristic update
        ArmsLengthProtocolMessage msg;
        if (msgHandler.receiveUniqueMessage(msg, pData, length))
        {
            switch ((FlapsCommands)msg.getCommand())
            {
            case FlapsCommands::CLOSE:
    Serial.println( F("RightLeg close flap") );
                changeFlapState(FlapState_e::Closing);
                break;

            case FlapsCommands::OPEN:
    Serial.println( F("RightLeg open flap") );
                changeFlapState(FlapState_e::Opening);
                break;

            default:
    Serial.println( F("RightLeg unknown back flaps command") );
                // Error
                break;
            }
        }
        else
        {
            Serial.print( F("RightLeg::onValueChanged back flaps receiveUniqueMessage failed") );
        }
    }
}
