
#include <Arduino.h>

#include "RightArmInputController.h"
#include "UUIDs.h"
#include "ArmsLengthProtocolMessage.h"


// For installation instructions see: https://github.com/mathertel/OneButton
#include "lib/OneButton/src/OneButton.cpp"

static void handleButtonSingleTap(void* param)
{
    RightArmInputController::AttachedButton* attachedButton = (RightArmInputController::AttachedButton*)param;
    attachedButton->controller->buttonPressed(attachedButton, 1);
}

static void handleButtonDoubleTap(void* param)
{
    RightArmInputController::AttachedButton* attachedButton = (RightArmInputController::AttachedButton*)param;
    attachedButton->controller->buttonPressed(attachedButton, 2);
}

static void handleButtonLongPress(void* param)
{
    RightArmInputController::AttachedButton* attachedButton = (RightArmInputController::AttachedButton*)param;
    attachedButton->controller->buttonPressed(attachedButton, RightArmInputController::LONGPRESS);
}

static void handleButtonMultiPress(void* param)
{
    RightArmInputController::AttachedButton* attachedButton = (RightArmInputController::AttachedButton*)param;
    int numPresses = attachedButton->button.getNumberClicks();
    attachedButton->controller->buttonPressed(attachedButton, numPresses);
}



RightArmInputController::RightArmInputController(
    BLECharacteristicRegistry& characteristicRegistry
    ):
    ArmsLengthController(characteristicRegistry),
    thumb(this, D0, true, true), // activeLow, pullup
    indexFinger(this, D1, true, true),
    middleFinger(this, D2, true, true),
    ringFinger(this, D3, true, true),
    pinkyFinger(this, D4, true, true)
{
    characteristicRegistry.attachCharacteristicRegisteredCallback(
        std::bind(
            &RightArmInputController::onCharacteristicRegistered,
            this,
            std::placeholders::_1
            )
        );
    // Don't attach any "value changed" callbacks, because we only update the
    // Characteristics and have no reason to be notified of their updates.
}

RightArmInputController::~RightArmInputController()
{
}

void RightArmInputController::setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    thumb.button.attachClick(handleButtonSingleTap, &thumb);
    thumb.button.attachDoubleClick(handleButtonDoubleTap, &thumb);
    thumb.button.attachLongPressStart(handleButtonLongPress, &thumb);
    thumb.button.attachMultiClick(handleButtonMultiPress, &thumb);

    indexFinger.button.attachClick(handleButtonSingleTap, &indexFinger);
    indexFinger.button.attachDoubleClick(handleButtonDoubleTap, &indexFinger);
    indexFinger.button.attachLongPressStart(handleButtonLongPress, &indexFinger);
    indexFinger.button.attachMultiClick(handleButtonMultiPress, &indexFinger);

    middleFinger.button.attachClick(handleButtonSingleTap, &middleFinger);
    middleFinger.button.attachDoubleClick(handleButtonDoubleTap, &middleFinger);
    middleFinger.button.attachLongPressStart(handleButtonLongPress, &middleFinger);
    middleFinger.button.attachMultiClick(handleButtonMultiPress, &middleFinger);

    ringFinger.button.attachClick(handleButtonSingleTap, &ringFinger);
    ringFinger.button.attachDoubleClick(handleButtonDoubleTap, &ringFinger);
    ringFinger.button.attachLongPressStart(handleButtonLongPress, &ringFinger);
    ringFinger.button.attachMultiClick(handleButtonMultiPress, &ringFinger);

    pinkyFinger.button.attachClick(handleButtonSingleTap, &pinkyFinger);
    pinkyFinger.button.attachDoubleClick(handleButtonDoubleTap, &pinkyFinger);
    pinkyFinger.button.attachLongPressStart(handleButtonLongPress, &pinkyFinger);
    pinkyFinger.button.attachMultiClick(handleButtonMultiPress, &pinkyFinger);
}

void RightArmInputController::loop()
{
//  Serial.println( F("thumb tick") );
    thumb.button.tick();
//  Serial.println( F("indexFinger tick") );
    indexFinger.button.tick();
//  Serial.println( F("middleFinger tick") );
    middleFinger.button.tick();
//  Serial.println( F("ringFinger tick") );
    ringFinger.button.tick();
//  Serial.println( F("pinkyFinger tick") );
    pinkyFinger.button.tick();
}

bool RightArmInputController::onCharacteristicRegistered(BLEUUID characteristicUUID)
{
    // TODO: Which of these will the right arm actually control?
    //       For example, will it run scripts, or are we leaving that to a phone, etc.?
    return 
        characteristicUUID.equals(UUIDs::UUID_DEFAULTS_SCRIPT_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_THREAT_DISPLAY_SCRIPT_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_ANGRY_SCRIPT_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_READY_TO_FLY_SCRIPT_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_FLAPS_TEST_SCRIPT_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_EYES_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_FACEPLATE_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_SPEECH_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_ARC_REACTOR_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_CHEST_FLAPS_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_BACK_FLAPS_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_RIGHT_ARM_LASER_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_RIGHT_ARM_ROCKET_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_RIGHT_ARM_REPULSOR_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_LEFT_ARM_LASER_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_LEFT_ARM_ROCKET_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_LEFT_ARM_REPULSOR_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_RIGHT_LEG_FLAPS_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_LEFT_LEG_FLAPS_CHARACTERISTIC);
};

void RightArmInputController::buttonPressed(AttachedButton* button, NumberOfPresses numPresses)
{
  Serial.print( F("button pressed ") );
  Serial.print(button->pin);
  Serial.print( F(" numpresses = ") );
  Serial.print(numPresses);

    if (button == &thumb)
    {
  Serial.println( F(" thumb") );
        switch (numPresses)
        {
        case LONGPRESS:
            //digitalWrite(LED_BUILTIN, HIGH);  // turn the LED OFF (active low)
            break;
        case 1:
            //digitalWrite(LED_BUILTIN, LOW);  // turn the LED ON (active low)
            break;
        case 2:
            toggleRightArmRocket();
            break;
        case 3:
            break;
        default:
            break;
        }
    }
    
    if (button == &indexFinger)
    {
  Serial.println( F(" index") );
        switch (numPresses)
        {
        case LONGPRESS:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            break;
        }
    }
    
    if (button == &middleFinger)
    {
  Serial.println( F(" middle") );
        switch (numPresses)
        {
        case LONGPRESS:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            break;
        }
    }
    
    if (button == &ringFinger)
    {
  Serial.println( F(" ring") );
        switch (numPresses)
        {
        case LONGPRESS:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            break;
        }
    }
    
    if (button == &pinkyFinger)
    {
  Serial.println( F(" pinky") );
        switch (numPresses)
        {
        case LONGPRESS:
            break;
        case 1:
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

void RightArmInputController::toggleRightArmRocket()
{
    bool isClosed = true;

    ArmsLengthProtocolMessage msg;
    uint8_t buf[sizeof(msg)];
    size_t numBytesReceived = 0;

    if (characteristicRegistry.getValue(UUIDs::UUID_RIGHT_ARM_ROCKET_CHARACTERISTIC, buf, sizeof(buf), numBytesReceived))
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

    msg.setService(Services::RIGHT_ARM_SERVICE);
    msg.setCharacteristic(Characteristics::RIGHT_ARM_ROCKET_CHARACTERISTIC);
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
    Serial.print( F("updateCharacteristic") );

        characteristicRegistry.updateCharacteristic(UUIDs::UUID_RIGHT_ARM_ROCKET_CHARACTERISTIC, buf, numBytesSerialized);
    }
    else
    {
    Serial.print( F("Error serializing message") );

        // Error
    }
}

