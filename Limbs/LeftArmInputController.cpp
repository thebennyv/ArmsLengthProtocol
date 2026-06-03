
#include <Arduino.h>

#include "LeftArmInputController.h"
#include "LeftArmPinout.h"
#include "UUIDs.h"
#include "ArmsLengthProtocolMessage.h"

namespace { // anonymous

static void handleButtonSingleTap(void* param)
{
    LeftArmInputController::AttachedButton* attachedButton = (LeftArmInputController::AttachedButton*)param;
    attachedButton->controller->buttonPressed(attachedButton, 1);
}

static void handleButtonDoubleTap(void* param)
{
    LeftArmInputController::AttachedButton* attachedButton = (LeftArmInputController::AttachedButton*)param;
    attachedButton->controller->buttonPressed(attachedButton, 2);
}

static void handleButtonLongPressStart(void* param)
{
    LeftArmInputController::AttachedButton* attachedButton = (LeftArmInputController::AttachedButton*)param;
    attachedButton->controller->buttonPressed(attachedButton, LeftArmInputController::LONGPRESS_START);
}

static void handleButtonLongPressStop(void* param)
{
    LeftArmInputController::AttachedButton* attachedButton = (LeftArmInputController::AttachedButton*)param;
    attachedButton->controller->buttonPressed(attachedButton, LeftArmInputController::LONGPRESS_STOP);
}

static void handleButtonMultiPress(void* param)
{
    LeftArmInputController::AttachedButton* attachedButton = (LeftArmInputController::AttachedButton*)param;
    int numPresses = attachedButton->button.getNumberClicks();
    attachedButton->controller->buttonPressed(attachedButton, numPresses);
}

} // namespace anonymous

LeftArmInputController::LeftArmInputController(
    BLECharacteristicRegistry& characteristicRegistry
    ):
    ArmsLengthController(characteristicRegistry),
    thumb(this, (int)LeftArmPin_e::ThumbButton, true, true), // activeLow, pullup
    flexSensorWatermarkOn(300),
    flexSensorWatermarkOff(3150),
    flexSenorTriggered(false)

{
    characteristicRegistry.attachCharacteristicRegisteredCallback(
        std::bind(
            &LeftArmInputController::onCharacteristicRegistered,
            this,
            std::placeholders::_1
            )
        );
    // Don't attach any "value changed" callbacks, because we only update the
    // Characteristics and have no reason to be notified of their updates.
}

LeftArmInputController::~LeftArmInputController()
{
}

void LeftArmInputController::setup()
{
    thumb.button.attachClick(handleButtonSingleTap, &thumb);
    thumb.button.attachDoubleClick(handleButtonDoubleTap, &thumb);
    thumb.button.attachLongPressStart(handleButtonLongPressStart, &thumb);
    thumb.button.attachLongPressStop(handleButtonLongPressStop, &thumb);
    thumb.button.attachMultiClick(handleButtonMultiPress, &thumb);
}

void LeftArmInputController::loop()
{
    thumb.button.tick();


    // Read flex sensor for repulsor
    int flexSensorReadingMv = analogReadMilliVolts((int)LeftArmPin_e::FlexSensor);
    if (!flexSenorTriggered)
    {
        if (flexSensorReadingMv < flexSensorWatermarkOn)
        {
            flexSenorTriggered = true;
            leftArmRepulsor(RepulsorCommands::CHARGE);
        }
    }
    else
    {
        if (flexSensorReadingMv > flexSensorWatermarkOff)
        {
            flexSenorTriggered = false;
            leftArmRepulsor(RepulsorCommands::OFF);
        }
    }
}

bool LeftArmInputController::onCharacteristicRegistered(BLEUUID characteristicUUID)
{
    // TODO: Which of these will the left arm actually control?
    //       For example, will it run scripts, or are we leaving that to a phone, etc.?
    return 
        /*characteristicUUID.equals(UUIDs::UUID_DEFAULTS_SCRIPT_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_THREAT_DISPLAY_SCRIPT_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_ANGRY_SCRIPT_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_READY_TO_FLY_SCRIPT_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_FLAPS_TEST_SCRIPT_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_EYES_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_FACEPLATE_CHARACTERISTIC) ||*/
        characteristicUUID.equals(UUIDs::UUID_AUDIO_CHARACTERISTIC) ||/*
        characteristicUUID.equals(UUIDs::UUID_ARC_REACTOR_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_CHEST_FLAPS_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_BACK_FLAPS_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_SHOULDER_FLAPS_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_LEFT_ARM_LASER_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_LEFT_ARM_ROCKET_CHARACTERISTIC) ||*/
        characteristicUUID.equals(UUIDs::UUID_LEFT_ARM_REPULSOR_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_LEFT_ARM_LASER_CHARACTERISTIC) /*||
        characteristicUUID.equals(UUIDs::UUID_LEFT_ARM_ROCKET_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_LEFT_ARM_REPULSOR_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_RIGHT_LEG_FLAPS_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_LEFT_LEG_FLAPS_CHARACTERISTIC)*/;
};

void LeftArmInputController::buttonPressed(AttachedButton* button, NumberOfPresses numPresses)
{
  Serial.print( F("button pressed ") );
  Serial.print(button->pin);
  Serial.print( F(" numpresses = ") );
  Serial.print(numPresses);

    if (button == &thumb)
    {
        switch (numPresses)
        {
        case LONGPRESS_START:
            leftArmLaser(LaserCommands::ON);
            break;
        case LONGPRESS_STOP:
            leftArmLaser(LaserCommands::OFF);
            break;
        case 1:
            toggleLeftArmRocket();
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            break;
        }
    }
}

void LeftArmInputController::leftArmLaser(LaserCommands command)
{
    ArmsLengthProtocolMessage msg;
    uint8_t buf[sizeof(msg)];

    msg.setService(Services::LEFT_ARM_SERVICE);
    msg.setCharacteristic(Characteristics::LEFT_ARM_LASER_CHARACTERISTIC);
    msg.setCommand((uint32_t)command);

    std::size_t numBytesSerialized = 0;
    if (msg.serialize(buf, sizeof(buf), numBytesSerialized))
    {
        characteristicRegistry.updateCharacteristic(UUIDs::UUID_LEFT_ARM_LASER_CHARACTERISTIC, buf, numBytesSerialized);
    }
    else
    {
        // Error
    }
}

void LeftArmInputController::leftArmRepulsor(RepulsorCommands command)
{

    ArmsLengthProtocolMessage msg;
    uint8_t buf[sizeof(msg)];

    if (command == RepulsorCommands::CHARGE)
    {
        msg.setService(Services::CHEST_SERVICE);
        msg.setCharacteristic(Characteristics::AUDIO_CHARACTERISTIC);
        msg.setCommand((uint32_t)AudioCommands::REPULSOR_CHARGE);

        std::size_t numBytesSerialized = 0;
        if (msg.serialize(buf, sizeof(buf), numBytesSerialized))
        {
            characteristicRegistry.updateCharacteristic(UUIDs::UUID_AUDIO_CHARACTERISTIC, buf, numBytesSerialized);
            delay(500);
        }
        else
        {
            // Error
        }
    }

    msg.setService(Services::LEFT_ARM_SERVICE);
    msg.setCharacteristic(Characteristics::LEFT_ARM_REPULSOR_CHARACTERISTIC);
    msg.setCommand((uint32_t)command);

    std::size_t numBytesSerialized = 0;
    if (msg.serialize(buf, sizeof(buf), numBytesSerialized))
    {
        characteristicRegistry.updateCharacteristic(UUIDs::UUID_LEFT_ARM_REPULSOR_CHARACTERISTIC, buf, numBytesSerialized);
    }
    else
    {
        // Error
    }

}


void LeftArmInputController::toggleLeftArmRocket()
{
    bool isClosed = true;

    ArmsLengthProtocolMessage msg;
    uint8_t buf[sizeof(msg)];
    size_t numBytesReceived = 0;

    if (characteristicRegistry.getValue(UUIDs::UUID_LEFT_ARM_ROCKET_CHARACTERISTIC, buf, sizeof(buf), numBytesReceived))
    {
        if (msg.deserialize(buf, numBytesReceived))
        {
            switch (msg.getCommand())
            {
            default: // assume it is closed and fall through
            case (uint32_t)RocketCommands::CLOSE:
                isClosed = true;
                break;
            case (uint32_t)RocketCommands::OPEN:
                isClosed = false;
                break;
            }
        }
    }

    msg.setService(Services::LEFT_ARM_SERVICE);
    msg.setCharacteristic(Characteristics::LEFT_ARM_ROCKET_CHARACTERISTIC);
    if (isClosed)
    {
        msg.setCommand((uint32_t)RocketCommands::OPEN);
    }
    else
    {
        msg.setCommand((uint32_t)RocketCommands::CLOSE);
    }

    std::size_t numBytesSerialized = 0;
    if (msg.serialize(buf, sizeof(buf), numBytesSerialized))
    {
        characteristicRegistry.updateCharacteristic(UUIDs::UUID_LEFT_ARM_ROCKET_CHARACTERISTIC, buf, numBytesSerialized);
    }
    else
    {
        // Error
    }
}
