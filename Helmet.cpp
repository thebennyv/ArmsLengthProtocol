#include "Helmet.h"
#include "HelmetPinout.h"
#include "UUIDs.h"
#include "ArmsLengthProtocolMessage.h"
#include "EasingFunctions.h"
#include <functional>

Helmet::Helmet(BLECharacteristicRegistry& characteristicRegistry):
    characteristicRegistry(characteristicRegistry),
    msgHandler(),
    faceplateServo1(),
    faceplateServo2(),
    faceplateServo1Pin((int)HelmetPin_e::FaceplateServo1),
    faceplateServo2Pin((int)HelmetPin_e::FaceplateServo2),
    faceplateState(),
    faceplateState_t0()
{
    characteristicRegistry.attachCharacteristicRegisteredCallback(
        std::bind(
            &Helmet::onCharacteristicRegistered,
            this,
            std::placeholders::_1
            )
        );
    characteristicRegistry.attachValueChangedCallback(
        std::bind(
            &Helmet::onValueChanged,
            this,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4
            )
        );
}

Helmet::~Helmet()
{
}   

void Helmet::setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

	// using default min/max of 1000us and 2000us
	// different servos may require different min/max settings
	// for an accurate 0 to 180 sweep
	faceplateServo1.setPeriodHertz(50); // standard 50 hz servo
	faceplateServo1.attach(faceplateServo1Pin, 1000, 2000);

	faceplateServo2.setPeriodHertz(50); // standard 50 hz servo
	faceplateServo2.attach(faceplateServo2Pin, 1000, 2000);
}

void Helmet::loop()
{
    loopFaceplate();
    loopEyes();
}

void Helmet::attachFaceplateServos()
{
    if (!faceplateServo1.attached())
    {
        faceplateServo1.attach(faceplateServo1Pin);
    }
    if (!faceplateServo2.attached())
    {
        faceplateServo2.attach(faceplateServo2Pin);
    }
}

void Helmet::detachFaceplateServos()
{
    if (faceplateServo1.attached())
    {
        faceplateServo1.detach();
    }
    if (faceplateServo2.attached())
    {
        faceplateServo2.detach();
    }
}

void Helmet::changeFaceplateState(FaceplateState_e newState)
{
    faceplateState_t0 = millis();
    faceplateState = newState;
}

double Helmet::scaleFaceplateServo1Angle(double norm_angle)
{
    // This is a subset of the Arduino map() function, which we could have used
    // like "map(norm_angle, 0.0, 1.0, ...Min, ...Max)" except that it
    // operates on integers instead of floating point values.

    const double outputMin = (double)FaceplateServoAngles_e::servo1Min;
    const double outputMax = (double)FaceplateServoAngles_e::servo1Max;
    const double outputRange = outputMax - outputMin;

    double scaled_angle = (norm_angle * outputRange) + outputMin;
    //Serial.println( "Helmet::scaleFaceplateServo1Angle norm_angle: " + String(norm_angle, 3) + " scaled_angle: " + String(scaled_angle, 3) );
    return scaled_angle;
}

double Helmet::scaleFaceplateServo2Angle(double norm_angle)
{
    // This is a subset of the Arduino map() function, which we could have used
    // like "map(norm_angle, 0.0, 1.0, ...Min, ...Max)" except that it
    // operates on integers instead of floating point values.

    const double outputMin = (double)FaceplateServoAngles_e::servo2Min;
    const double outputMax = (double)FaceplateServoAngles_e::servo2Max;
    const double outputRange = outputMax - outputMin;

    double scaled_angle = (norm_angle * outputRange) + outputMin;
    //Serial.println( "Helmet::scaleFaceplateServo2Angle norm_angle: " + String(norm_angle, 3) + " scaled_angle: " + String(scaled_angle, 3) );
    return scaled_angle;
}

void Helmet::loopFaceplate()
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
    const unsigned long dt = currentTime - faceplateState_t0;

    switch (faceplateState)
    {
    case FaceplateState_e::Closed:
    {
        // todo should we periodically attach and pulse the servos to ensure things stay put?
        detachFaceplateServos();
        break;
    }
    case FaceplateState_e::Opening:
    {
        const double norm_dt = (double)dt / (double)FaceplateStateDurations_e::DurationOpening;
        if (norm_dt <= 1.0)
        {
            attachFaceplateServos();

            double norm_faceplateServo1Angle = Easing::easeInOutLinear(norm_dt);
            double norm_faceplateServo2Angle = Easing::easeInOutLinear(norm_dt);
            double faceplateServo1Angle = scaleFaceplateServo1Angle(norm_faceplateServo1Angle);
            double faceplateServo2Angle = scaleFaceplateServo2Angle(norm_faceplateServo2Angle);
            faceplateServo1.write((int)faceplateServo1Angle);
            faceplateServo2.write((int)faceplateServo2Angle);
        }
        if (norm_dt >= 1.0)
        {
            changeFaceplateState(FaceplateState_e::HoldingOpen);
        }
        break;
    }
    case FaceplateState_e::HoldingOpen:
    {
        const double norm_dt = (double)dt / (double)FaceplateStateDurations_e::DurationHoldingOpen;
        if (norm_dt <= 1.0)
        {
            attachFaceplateServos();

            faceplateServo1.write((int)FaceplateServoAngles_e::servo1Max);
            faceplateServo2.write((int)FaceplateServoAngles_e::servo2Max);
        }
        if (norm_dt >= 1.0)
        {
            detachFaceplateServos(); // todo should we continue to hold the cover open while the arm opens, to ensure it doesn't accidentally retract?

            changeFaceplateState(FaceplateState_e::Opened);
        }
        break;
    }
    case FaceplateState_e::Opened:
    {
        // todo should we periodically attach and pulse the servos to ensure things stay put?
        detachFaceplateServos();
        break;
    }
    case FaceplateState_e::Closing:
    {
        const double norm_dt = (double)dt / (double)FaceplateStateDurations_e::DurationClosing;
        if (norm_dt <= 1.0)
        {
            attachFaceplateServos();

            double norm_faceplateServo1Angle = Easing::easeInOutLinear(1.0 - norm_dt); // 1.0-dt because we are going in reverse
            double norm_faceplateServo2Angle = Easing::easeInOutLinear(1.0 - norm_dt);
            double faceplateServo1Angle = scaleFaceplateServo1Angle(norm_faceplateServo1Angle);
            double faceplateServo2Angle = scaleFaceplateServo2Angle(norm_faceplateServo2Angle);
            faceplateServo1.write((int)faceplateServo1Angle);
            faceplateServo2.write((int)faceplateServo2Angle);
        }
        if (norm_dt >= 1.0)
        {
            changeFaceplateState(FaceplateState_e::HoldingClosed);
        }
        break;
    }
    case FaceplateState_e::HoldingClosed:
    {
        const double norm_dt = (double)dt / (double)FaceplateStateDurations_e::DurationHoldingClosed;
        if (norm_dt <= 1.0)
        {
            attachFaceplateServos();

            faceplateServo1.write((int)FaceplateServoAngles_e::servo1Min);
            faceplateServo2.write((int)FaceplateServoAngles_e::servo2Min);
        }
        if (norm_dt >= 1.0)
        {
            detachFaceplateServos();

            changeFaceplateState(FaceplateState_e::Closed);
        }
        break;
    }
    default:
        // Error
        break;
    }
}

void Helmet::loopEyes()
{
}

bool Helmet::onCharacteristicRegistered(BLEUUID characteristicUUID)
{
    return 
        characteristicUUID.equals(UUIDs::UUID_EYES_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_FACEPLATE_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_SPEECH_CHARACTERISTIC);
};

void Helmet::onValueChanged(
    BLEUUID characteristicUUID,
    uint8_t* pData,
    size_t length,
    bool isNotify)
{
    Serial.print( F("Helmet::onValueChanged") );

    if (characteristicUUID.equals(UUIDs::UUID_EYES_CHARACTERISTIC))
    {
        // Handle Eyes Characteristic update
    }
    else if (characteristicUUID.equals(UUIDs::UUID_FACEPLATE_CHARACTERISTIC))
    {
    Serial.println( F("Helmet handling UUID_FACEPLATE_CHARACTERISTIC") );

        // Handle Faceplate Characteristic update
        ArmsLengthProtocolMessage msg;
        if (msgHandler.receiveUniqueMessage(msg, pData, length))
        {
            switch ((FaceplateCommands)msg.getCommand())
            {
            case FaceplateCommands::CLOSE:
    Serial.println( F("Helmet close faceplate") );
                digitalWrite(LED_BUILTIN, HIGH);  // turn the LED OFF (active low)
                changeFaceplateState(FaceplateState_e::Closing);
                break;

            case FaceplateCommands::OPEN:
    Serial.println( F("Helmet open faceplate") );
                digitalWrite(LED_BUILTIN, LOW);  // turn the LED ON (active low)
                changeFaceplateState(FaceplateState_e::Opening);
                break;

            default:
    Serial.println( F("Helmet unknown faceplate command") );
                // Error
                break;
            }
        }
        else
        {
            Serial.print( F("Helmet::onValueChanged receiveUniqueMessage failed") );
        }
    }
    else if (characteristicUUID.equals(UUIDs::UUID_SPEECH_CHARACTERISTIC))
    {
        // Handle Speech Characteristic update
    }
}
