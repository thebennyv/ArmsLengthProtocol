#include "RightArm.h"
#include "UUIDs.h"
#include "ArmsLengthProtocolMessage.h"
#include "EasingFunctions.h"
#include <functional>

RightArm::RightArm(BLECharacteristicRegistry& characteristicRegistry):
    characteristicRegistry(characteristicRegistry),
    msgHandler(),
    rocketCover(),
    rocketCoverServoPin(D7), // todo
    rocketArm(),
    rocketArmServoPin(D8), // todo
    rocketState(),
    rocketState_t0()
{
    characteristicRegistry.attachCharacteristicRegisteredCallback(
        std::bind(
            &RightArm::onCharacteristicRegistered,
            this,
            std::placeholders::_1
            )
        );
    characteristicRegistry.attachValueChangedCallback(
        std::bind(
            &RightArm::onValueChanged,
            this,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4
            )
        );
}

RightArm::~RightArm()
{
}   

void RightArm::setup()
{
//    pinMode(LED_BUILTIN, OUTPUT);

	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
//	ESP32PWM::allocateTimer(2);
//	ESP32PWM::allocateTimer(3);

	// using default min/max of 1000us and 2000us
	// different servos may require different min/max settings
	// for an accurate 0 to 180 sweep
	rocketCover.setPeriodHertz(50); // standard 50 hz servo
	rocketCover.attach(rocketCoverServoPin, 1000, 2000);

	rocketArm.setPeriodHertz(50); // standard 50 hz servo
	rocketArm.attach(rocketArmServoPin, 1000, 2000);
}

void RightArm::loop()
{
//    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on

    loopLaser();
    loopRocket();
    loopRepulsor();
}

void RightArm::attachRocketCover()
{
    if (!rocketCover.attached())
    {
        rocketCover.attach(rocketCoverServoPin);
    }
}

void RightArm::detachRocketCover()
{
    if (rocketCover.attached())
    {
        rocketCover.detach();
    }
}

void RightArm::attachRocketArm()
{
    if (!rocketArm.attached())
    {
        rocketArm.attach(rocketArmServoPin);
    }
}

void RightArm::detachRocketArm()
{
    if (rocketArm.attached())
    {
        rocketArm.detach();
    }
}

void RightArm::changeRocketState(RocketState_e newState)
{
    rocketState = newState;
    rocketState_t0 = millis();
}

double RightArm::scaleRocketCoverAngle(double norm_coverAngle)
{
    // This is a subset of the Arduino map() function, which we could have used
    // like "map(norm_coverAngle, 0.0, 1.0, ...Min, ...Max)" except that it
    // operates on integers instead of floating point values.

    const double outputMin = (double)RocketCoverAngles_e::Min;
    const double outputMax = (double)RocketCoverAngles_e::Max;
    const double outputRange = outputMax - outputMin;

    return (norm_coverAngle * outputRange) + outputMin;
}

double RightArm::scaleRocketArmAngle(double norm_armAngle)
{
    const double outputMin = (double)RocketArmAngles_e::Min;
    const double outputMax = (double)RocketArmAngles_e::Max;
    const double outputRange = outputMax - outputMin;

    return (norm_armAngle * outputRange) + outputMin;
}

void RightArm::loopLaser()
{
}

void RightArm::loopRocket()
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
    const unsigned long dt = currentTime - rocketState_t0;

    switch (rocketState)
    {
    case RocketState_e::Closed:
    {
        // todo should we periodically attach and pulse the servos to ensure things stay put?
        detachRocketCover();
        detachRocketArm();
        break;
    }
    case RocketState_e::OpeningCover:
    {
        const double norm_dt = (double)dt / (double)RocketStateDurations_e::DurationOpeningCover;
        if (norm_dt <= 1.0)
        {
            attachRocketCover();

            double norm_coverAngle = Easing::easeInOutCubic(norm_dt);
            double coverAngle = scaleRocketCoverAngle(norm_coverAngle);
            rocketCover.write((int)coverAngle);
        }
        if (norm_dt >= 1.0)
        {
            changeRocketState(RocketState_e::HoldingCoverOpen);
        }
        break;
    }
    case RocketState_e::HoldingCoverOpen:
    {
        const double norm_dt = (double)dt / (double)RocketStateDurations_e::DurationHoldingCoverOpen;
        if (norm_dt <= 1.0)
        {
            attachRocketCover();

            rocketCover.write((int)RocketCoverAngles_e::Max);
        }
        if (norm_dt >= 1.0)
        {
            detachRocketCover(); // todo should we continue to hold the cover open while the arm opens, to ensure it doesn't accidentally retract?

            changeRocketState(RocketState_e::OpeningRocket);
        }
        break;
    }
    case RocketState_e::OpeningRocket:
    {
        const double norm_dt = (double)dt / (double)RocketStateDurations_e::DurationOpeningRocket;
        if (norm_dt <= 1.0)
        {
            attachRocketArm();

            double norm_armAngle = Easing::easeInOutCubic(norm_dt);
            double armAngle = scaleRocketCoverAngle(norm_armAngle);
            rocketCover.write((int)armAngle);
        }
        if (norm_dt >= 1.0)
        {
            changeRocketState(RocketState_e::HoldingRocketOpen);
        }
        break;
    }
    case RocketState_e::HoldingRocketOpen:
    {
        const double norm_dt = (double)dt / (double)RocketStateDurations_e::DurationHoldingRocketOpen;
        if (norm_dt <= 1.0)
        {
            attachRocketArm();

            rocketArm.write((int)RocketArmAngles_e::Max);
        }
        if (norm_dt >= 1.0)
        {
            detachRocketArm();

            changeRocketState(RocketState_e::Opened);
        }
        break;
    }
    case RocketState_e::Opened:
    {
        // todo should we periodically attach and pulse the servos to ensure things stay put?
        detachRocketCover();
        detachRocketArm();
        break;
    }
    case RocketState_e::ClosingRocket:
    {
        const double norm_dt = (double)dt / (double)RocketStateDurations_e::DurationClosingRocket;
        if (norm_dt <= 1.0)
        {
            attachRocketArm();

            double norm_armAngle = Easing::easeInOutCubic(1.0 - norm_dt); // 1.0-dt because we are going in reverse
            double armAngle = scaleRocketCoverAngle(norm_armAngle);
            rocketCover.write((int)armAngle);
        }
        if (norm_dt >= 1.0)
        {
            changeRocketState(RocketState_e::HoldingRocketClosed);
        }
        break;
    }
    case RocketState_e::HoldingRocketClosed:
    {
        const double norm_dt = (double)dt / (double)RocketStateDurations_e::DurationHoldingRocketClosed;
        if (norm_dt <= 1.0)
        {
            attachRocketArm();

            rocketArm.write((int)RocketArmAngles_e::Min);
        }
        if (norm_dt >= 1.0)
        {
            detachRocketArm();

            changeRocketState(RocketState_e::ClosingCover);
        }
        break;
    }
    case RocketState_e::ClosingCover:
    {
        const double norm_dt = (double)dt / (double)RocketStateDurations_e::DurationClosingCover;
        if (norm_dt <= 1.0)
        {
            double norm_coverAngle = Easing::easeInOutCubic(1.0 - norm_dt); // 1.0-dt because we are going in reverse
            double coverAngle = scaleRocketCoverAngle(norm_coverAngle);
            rocketCover.write((int)coverAngle);
        }
        if (norm_dt >= 1.0)
        {
            changeRocketState(RocketState_e::HoldingCoverClosed);
        }
        break;
    }
    case RocketState_e::HoldingCoverClosed:
    {
        const double norm_dt = (double)dt / (double)RocketStateDurations_e::DurationHoldingCoverClosed;
        if (norm_dt <= 1.0)
        {
            attachRocketCover();

            rocketCover.write((int)RocketCoverAngles_e::Min);
        }
        if (norm_dt >= 1.0)
        {
            detachRocketCover();

            changeRocketState(RocketState_e::Closed);
        }
        break;
    }
    default:
        // Error
        break;
    }
}

void RightArm::loopRepulsor()
{
}

bool RightArm::onCharacteristicRegistered(BLEUUID characteristicUUID)
{
    return 
        characteristicUUID.equals(UUIDs::UUID_RIGHT_ARM_LASER_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_RIGHT_ARM_ROCKET_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_RIGHT_ARM_REPULSOR_CHARACTERISTIC);
};

void RightArm::onValueChanged(
    BLEUUID characteristicUUID,
    uint8_t* pData,
    size_t length,
    bool isNotify)
{
    Serial.print( F("RightArm::onValueChanged") );

    if (characteristicUUID.equals(UUIDs::UUID_RIGHT_ARM_LASER_CHARACTERISTIC))
    {
        // Handle Right Arm Laser Characteristic update
        ArmsLengthProtocolMessage msg;
        if (msgHandler.receiveUniqueMessage(msg, pData, length))
        {
            switch ((LaserCommands)msg.getCommand())
            {
            case LaserCommands::OFF:
                break;
            case LaserCommands::ON:
                break;
            default:
                // Error
                break;
            }
        }
    }
    else if (characteristicUUID.equals(UUIDs::UUID_RIGHT_ARM_ROCKET_CHARACTERISTIC))
    {
    Serial.println( F("RightArm handling UUID_RIGHT_ARM_ROCKET_CHARACTERISTIC") );

        // Handle Right Arm Rocket Characteristic update
        ArmsLengthProtocolMessage msg;
        if (msgHandler.receiveUniqueMessage(msg, pData, length))
        {
            switch ((RocketCommands)msg.getCommand())
            {
            case RocketCommands::CLOSE:
    Serial.println( F("RightArm close rocket") );
                digitalWrite(LED_BUILTIN, HIGH);  // turn the LED OFF (active low)
                changeRocketState(RocketState_e::ClosingRocket);
                break;

            case RocketCommands::OPEN:
    Serial.println( F("RightArm open rocket") );
                digitalWrite(LED_BUILTIN, LOW);  // turn the LED ON (active low)
                changeRocketState(RocketState_e::OpeningCover);
                break;

            default:
    Serial.println( F("RightArm unknown rocket command") );
                // Error
                break;
            }
        }
        else
        {
            Serial.print( F("RightArm::onValueChanged receiveUniqueMessage failed") );
        }
    }
    else if (characteristicUUID.equals(UUIDs::UUID_RIGHT_ARM_REPULSOR_CHARACTERISTIC))
    {
        // Handle Right Arm Repulsor Characteristic update
        ArmsLengthProtocolMessage msg;
        if (msgHandler.receiveUniqueMessage(msg, pData, length))
        {
            switch ((RepulsorCommands)msg.getCommand())
            {
            case RepulsorCommands::OFF:
                break;
            case RepulsorCommands::ON:
                break;
            case RepulsorCommands::CHARGE:
                break;
            case RepulsorCommands::FIRE:
                break;
            default:
                // Error
                break;
            }
        }
    }
}
