
#include <Arduino.h>

#include "RightArmInputController.h"
#include "RightArmPinout.h"
#include "UUIDs.h"
#include "ArmsLengthProtocolMessage.h"

namespace { // anonymous

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

static void handleButtonLongPressStart(void* param)
{
    RightArmInputController::AttachedButton* attachedButton = (RightArmInputController::AttachedButton*)param;
    attachedButton->controller->buttonPressed(attachedButton, RightArmInputController::LONGPRESS_START);
}

static void handleButtonLongPressStop(void* param)
{
    RightArmInputController::AttachedButton* attachedButton = (RightArmInputController::AttachedButton*)param;
    attachedButton->controller->buttonPressed(attachedButton, RightArmInputController::LONGPRESS_STOP);
}

static void handleButtonMultiPress(void* param)
{
    RightArmInputController::AttachedButton* attachedButton = (RightArmInputController::AttachedButton*)param;
    int numPresses = attachedButton->button.getNumberClicks();
    attachedButton->controller->buttonPressed(attachedButton, numPresses);
}

} // namespace anonymous

RightArmInputController::RightArmInputController(
    BLECharacteristicRegistry& characteristicRegistry
    ):
    ArmsLengthController(characteristicRegistry),
    thumb(this, (int)RightArmPin_e::ThumbButton, true, true), // activeLow, pullup
    indexFinger(this, (int)RightArmPin_e::IndexFingerButton, true, true),
    middleFinger(this, (int)RightArmPin_e::MiddleFingerButton, true, true),
    ringFinger(this, (int)RightArmPin_e::RingFingerButton, true, true),
    flexSensorWatermarkOn(300),
    flexSensorWatermarkOff(3150),
    flexSenorTriggered(false)
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
    thumb.button.attachClick(handleButtonSingleTap, &thumb);
    thumb.button.attachDoubleClick(handleButtonDoubleTap, &thumb);
    thumb.button.attachLongPressStart(handleButtonLongPressStart, &thumb);
    thumb.button.attachLongPressStop(handleButtonLongPressStop, &thumb);
    thumb.button.attachMultiClick(handleButtonMultiPress, &thumb);

    indexFinger.button.attachClick(handleButtonSingleTap, &indexFinger);
    indexFinger.button.attachDoubleClick(handleButtonDoubleTap, &indexFinger);
    indexFinger.button.attachLongPressStart(handleButtonLongPressStart, &indexFinger);
    indexFinger.button.attachLongPressStop(handleButtonLongPressStop, &indexFinger);
    indexFinger.button.attachMultiClick(handleButtonMultiPress, &indexFinger);

    middleFinger.button.attachClick(handleButtonSingleTap, &middleFinger);
    middleFinger.button.attachDoubleClick(handleButtonDoubleTap, &middleFinger);
    middleFinger.button.attachLongPressStart(handleButtonLongPressStart, &middleFinger);
    middleFinger.button.attachLongPressStop(handleButtonLongPressStop, &middleFinger);
    middleFinger.button.attachMultiClick(handleButtonMultiPress, &middleFinger);

    ringFinger.button.attachClick(handleButtonSingleTap, &ringFinger);
    ringFinger.button.attachDoubleClick(handleButtonDoubleTap, &ringFinger);
    ringFinger.button.attachLongPressStart(handleButtonLongPressStart, &ringFinger);
    ringFinger.button.attachLongPressStop(handleButtonLongPressStop, &ringFinger);
    ringFinger.button.attachMultiClick(handleButtonMultiPress, &ringFinger);
}

void RightArmInputController::loop()
{
    thumb.button.tick();
    indexFinger.button.tick();
    middleFinger.button.tick();
    ringFinger.button.tick();


    // Read flex sensor for repulsor
    int flexSensorReadingMv = 3000; //analogReadMilliVolts((int)RightArmPin_e::FlexSensor);
    if (!flexSenorTriggered)
    {
        if (flexSensorReadingMv < flexSensorWatermarkOn)
        {
            flexSenorTriggered = true;
            rightArmRepulsor(RepulsorCommands::CHARGE);
        }
    }
    else
    {
        if (flexSensorReadingMv > flexSensorWatermarkOff)
        {
            flexSenorTriggered = false;
            rightArmRepulsor(RepulsorCommands::OFF);
        }
    }
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
        characteristicUUID.equals(UUIDs::UUID_AUDIO_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_ARC_REACTOR_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_CHEST_FLAPS_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_BACK_FLAPS_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_SHOULDER_FLAPS_CHARACTERISTIC) ||
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
        case LONGPRESS_START:
            rightArmLaser(LaserCommands::ON);
            break;
        case LONGPRESS_STOP:
            rightArmLaser(LaserCommands::OFF);
            break;
        case 1:
            toggleRightArmRocket();
            break;
        case 2:
            unibeam();
            break;
        case 3:
            break;
        default:
            break;
        }
    }
    
    if (button == &indexFinger)
    {
  Serial.println( F(" \n index") );
        switch (numPresses)
        {
        case LONGPRESS_START:
            dimEyes();
            break;
        case LONGPRESS_STOP:
            break;
        case 1:
            toggleFaceplate();
            break;
        case 2:
            toggleEyes();
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
        case LONGPRESS_START:
            resetReactor();
            break;
        case LONGPRESS_STOP:
            break;
        case 1:
            //toggleShoulderFlaps();
            {
                static bool on = false;
                if (on)
                {
rightArmRepulsor(RepulsorCommands::OFF);
on = false;
                }
                else
                {
rightArmRepulsor(RepulsorCommands::CHARGE);
on = true;

                }
            }
            
            break;
        case 2:
            toggleAllBackFlaps();
            break;
        case 3:
            partyMode();
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
        case LONGPRESS_START:
            break;
        case LONGPRESS_STOP:
            break;
        case 1:
            toggleChestFlaps();
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

void RightArmInputController::rightArmLaser(LaserCommands command)
{
    ArmsLengthProtocolMessage msg;
    uint8_t buf[sizeof(msg)];

    msg.setService(Services::RIGHT_ARM_SERVICE);
    msg.setCharacteristic(Characteristics::RIGHT_ARM_LASER_CHARACTERISTIC);
    msg.setCommand((uint32_t)command);

    std::size_t numBytesSerialized = 0;
    if (msg.serialize(buf, sizeof(buf), numBytesSerialized))
    {
        characteristicRegistry.updateCharacteristic(UUIDs::UUID_RIGHT_ARM_LASER_CHARACTERISTIC, buf, numBytesSerialized);
    }
    else
    {
        // Error
    }
}

void RightArmInputController::rightArmRepulsor(RepulsorCommands command)
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

    msg.setService(Services::RIGHT_ARM_SERVICE);
    msg.setCharacteristic(Characteristics::RIGHT_ARM_REPULSOR_CHARACTERISTIC);
    msg.setCommand((uint32_t)command);

    std::size_t numBytesSerialized = 0;
    if (msg.serialize(buf, sizeof(buf), numBytesSerialized))
    {
        characteristicRegistry.updateCharacteristic(UUIDs::UUID_RIGHT_ARM_REPULSOR_CHARACTERISTIC, buf, numBytesSerialized);
    }
    else
    {
        // Error
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
        characteristicRegistry.updateCharacteristic(UUIDs::UUID_RIGHT_ARM_ROCKET_CHARACTERISTIC, buf, numBytesSerialized);
    }
    else
    {
        // Error
    }
}

void RightArmInputController::dimEyes()
{
    // todo
}

void RightArmInputController::toggleFaceplate()
{
    bool isClosed = true;

    ArmsLengthProtocolMessage msg;
    uint8_t buf[sizeof(msg)];
    size_t numBytesReceived = 0;

    if (characteristicRegistry.getValue(UUIDs::UUID_FACEPLATE_CHARACTERISTIC, buf, sizeof(buf), numBytesReceived))
    {
        if (msg.deserialize(buf, numBytesReceived))
        {
            switch (msg.getCommand())
            {
            default: // assume it is closed and fall through
            case (uint32_t)FaceplateCommands::CLOSE:
                isClosed = true;
                break;
            case (uint32_t)FaceplateCommands::OPEN:
                isClosed = false;
                break;
            }
        }
    }

    msg.setService(Services::HELMET_SERVICE);
    msg.setCharacteristic(Characteristics::FACEPLATE_CHARACTERISTIC);
    if (isClosed)
    {
        msg.setCommand((uint32_t)FaceplateCommands::OPEN);
    }
    else
    {
        msg.setCommand((uint32_t)FaceplateCommands::CLOSE);
    }

    std::size_t numBytesSerialized = 0;
    if (msg.serialize(buf, sizeof(buf), numBytesSerialized))
    {
        characteristicRegistry.updateCharacteristic(UUIDs::UUID_FACEPLATE_CHARACTERISTIC, buf, numBytesSerialized);
    }
    else
    {
        // Error
    }

    if (!isClosed) // so, we are closing it now
    {
        delay(350);

        msg.setService(Services::CHEST_SERVICE);
        msg.setCharacteristic(Characteristics::AUDIO_CHARACTERISTIC);
        msg.setCommand((uint32_t)AudioCommands::CLANG);

        if (msg.serialize(buf, sizeof(buf), numBytesSerialized))
        {
            characteristicRegistry.updateCharacteristic(UUIDs::UUID_AUDIO_CHARACTERISTIC, buf, numBytesSerialized);
            //delay(500);
        }
        else
        {
            // Error
        }
    }
}

void RightArmInputController::toggleEyes()
{
    bool eyesAreOn = true;

    ArmsLengthProtocolMessage msg;
    uint8_t buf[sizeof(msg)];
    size_t numBytesReceived = 0;

    if (characteristicRegistry.getValue(UUIDs::UUID_EYES_CHARACTERISTIC, buf, sizeof(buf), numBytesReceived))
    {
        if (msg.deserialize(buf, numBytesReceived))
        {
            switch (msg.getCommand())
            {
            default: // assume it is closed and fall through
            case (uint32_t)EyesCommands::OFF:
                eyesAreOn = false;
                break;
            case (uint32_t)EyesCommands::ON:
                eyesAreOn = true;
                break;
            }
        }
    }

    msg.setService(Services::HELMET_SERVICE);
    msg.setCharacteristic(Characteristics::EYES_CHARACTERISTIC);
    if (eyesAreOn)
    {
        msg.setCommand((uint32_t)EyesCommands::OFF);
    }
    else
    {
        msg.setCommand((uint32_t)EyesCommands::ON);
    }

    std::size_t numBytesSerialized = 0;
    if (msg.serialize(buf, sizeof(buf), numBytesSerialized))
    {
        characteristicRegistry.updateCharacteristic(UUIDs::UUID_EYES_CHARACTERISTIC, buf, numBytesSerialized);
    }
    else
    {
        // Error
    }
}

void RightArmInputController::resetReactor()
{
    ArmsLengthProtocolMessage msg;
    uint8_t buf[sizeof(msg)];

    msg.setService(Services::CHEST_SERVICE);
    msg.setCharacteristic(Characteristics::ARC_REACTOR_CHARACTERISTIC);
    msg.setCommand((uint32_t)ArcReactorCommands::FLICKER_ON);

    std::size_t numBytesSerialized = 0;
    if (msg.serialize(buf, sizeof(buf), numBytesSerialized))
    {
        characteristicRegistry.updateCharacteristic(UUIDs::UUID_ARC_REACTOR_CHARACTERISTIC, buf, numBytesSerialized);
    }
    else
    {
        // Error
    }
}

void RightArmInputController::unibeam()
{
    ArmsLengthProtocolMessage msg;
    uint8_t buf[sizeof(msg)];
    std::size_t numBytesSerialized = 0;

    msg.setService(Services::CHEST_SERVICE);
    msg.setCharacteristic(Characteristics::AUDIO_CHARACTERISTIC);
    msg.setCommand((uint32_t)AudioCommands::UNIBEAM_CHARGE_AND_FIRE);

    if (msg.serialize(buf, sizeof(buf), numBytesSerialized))
    {
        characteristicRegistry.updateCharacteristic(UUIDs::UUID_AUDIO_CHARACTERISTIC, buf, numBytesSerialized);
        //delay(500);
    }
    else
    {
        // Error
    }

    msg.setService(Services::CHEST_SERVICE);
    msg.setCharacteristic(Characteristics::ARC_REACTOR_CHARACTERISTIC);
    msg.setCommand((uint32_t)ArcReactorCommands::UNIBEAM);

    if (msg.serialize(buf, sizeof(buf), numBytesSerialized))
    {
        characteristicRegistry.updateCharacteristic(UUIDs::UUID_ARC_REACTOR_CHARACTERISTIC, buf, numBytesSerialized);
    }
    else
    {
        // Error
    }
}

void RightArmInputController::toggleShoulderFlaps()
{
    bool isClosed = true;

    ArmsLengthProtocolMessage msg;
    uint8_t buf[sizeof(msg)];
    size_t numBytesReceived = 0;

    if (characteristicRegistry.getValue(UUIDs::UUID_SHOULDER_FLAPS_CHARACTERISTIC, buf, sizeof(buf), numBytesReceived))
    {
        if (msg.deserialize(buf, numBytesReceived))
        {
            switch (msg.getCommand())
            {
            default: // assume it is closed and fall through
            case (uint32_t)FlapsCommands::CLOSE:
                isClosed = true;
                break;
            case (uint32_t)FlapsCommands::OPEN:
                isClosed = false;
                break;
            }
        }
    }

    msg.setService(Services::BACK_SERVICE);
    msg.setCharacteristic(Characteristics::SHOULDER_FLAPS_CHARACTERISTIC);
    if (isClosed)
    {
        msg.setCommand((uint32_t)FlapsCommands::OPEN);
    }
    else
    {
        msg.setCommand((uint32_t)FlapsCommands::CLOSE);
    }

    std::size_t numBytesSerialized = 0;
    if (msg.serialize(buf, sizeof(buf), numBytesSerialized))
    {
        characteristicRegistry.updateCharacteristic(UUIDs::UUID_SHOULDER_FLAPS_CHARACTERISTIC, buf, numBytesSerialized);
    }
    else
    {
        // Error
    }
}

void RightArmInputController::toggleAllBackFlaps()
{
    bool isClosed = true;

    ArmsLengthProtocolMessage msg;
    uint8_t buf[sizeof(msg)];
    size_t numBytesReceived = 0;
    std::size_t numBytesSerialized = 0;

    if (characteristicRegistry.getValue(UUIDs::UUID_BACK_FLAPS_CHARACTERISTIC, buf, sizeof(buf), numBytesReceived))
    {
        if (msg.deserialize(buf, numBytesReceived))
        {
            switch (msg.getCommand())
            {
            default: // assume it is closed and fall through
            case (uint32_t)FlapsCommands::CLOSE:
                isClosed = true;
                break;
            case (uint32_t)FlapsCommands::OPEN:
                isClosed = false;
                break;
            }
        }
    }

    msg.setService(Services::BACK_SERVICE);
    msg.setCharacteristic(Characteristics::BACK_FLAPS_CHARACTERISTIC);
    if (isClosed)
    {
        msg.setCommand((uint32_t)FlapsCommands::OPEN);
    }
    else
    {
        msg.setCommand((uint32_t)FlapsCommands::CLOSE);
    }

    if (msg.serialize(buf, sizeof(buf), numBytesSerialized))
    {
        characteristicRegistry.updateCharacteristic(UUIDs::UUID_BACK_FLAPS_CHARACTERISTIC, buf, numBytesSerialized);
    }
    else
    {
        // Error
    }

    // Keep the leg flaps in sync

    msg.setService(Services::LEFT_LEG_SERVICE);
    msg.setCharacteristic(Characteristics::LEFT_LEG_FLAPS_CHARACTERISTIC);
    if (isClosed)
    {
        msg.setCommand((uint32_t)FlapsCommands::OPEN);
    }
    else
    {
        msg.setCommand((uint32_t)FlapsCommands::CLOSE);
    }

    if (msg.serialize(buf, sizeof(buf), numBytesSerialized))
    {
        characteristicRegistry.updateCharacteristic(UUIDs::UUID_LEFT_LEG_FLAPS_CHARACTERISTIC, buf, numBytesSerialized);
    }
    else
    {
        // Error
    }

    msg.setService(Services::RIGHT_LEG_SERVICE);
    msg.setCharacteristic(Characteristics::RIGHT_LEG_FLAPS_CHARACTERISTIC);
    if (isClosed)
    {
        msg.setCommand((uint32_t)FlapsCommands::OPEN);
    }
    else
    {
        msg.setCommand((uint32_t)FlapsCommands::CLOSE);
    }

    if (msg.serialize(buf, sizeof(buf), numBytesSerialized))
    {
        characteristicRegistry.updateCharacteristic(UUIDs::UUID_RIGHT_LEG_FLAPS_CHARACTERISTIC, buf, numBytesSerialized);
    }
    else
    {
        // Error
    }
}

void RightArmInputController::partyMode()
{
    ArmsLengthProtocolMessage msg;
    uint8_t buf[sizeof(msg)];

    msg.setService(Services::CHEST_SERVICE);
    msg.setCharacteristic(Characteristics::ARC_REACTOR_CHARACTERISTIC);
    msg.setCommand((uint32_t)ArcReactorCommands::PARTY);

    std::size_t numBytesSerialized = 0;
    if (msg.serialize(buf, sizeof(buf), numBytesSerialized))
    {
        characteristicRegistry.updateCharacteristic(UUIDs::UUID_ARC_REACTOR_CHARACTERISTIC, buf, numBytesSerialized);
    }
    else
    {
        // Error
    }
}

void RightArmInputController::toggleChestFlaps()
{
    bool isClosed = true;

    ArmsLengthProtocolMessage msg;
    uint8_t buf[sizeof(msg)];
    size_t numBytesReceived = 0;

    if (characteristicRegistry.getValue(UUIDs::UUID_CHEST_FLAPS_CHARACTERISTIC, buf, sizeof(buf), numBytesReceived))
    {
        if (msg.deserialize(buf, numBytesReceived))
        {
            switch (msg.getCommand())
            {
            default: // assume it is closed and fall through
            case (uint32_t)FlapsCommands::CLOSE:
                isClosed = true;
                break;
            case (uint32_t)FlapsCommands::OPEN:
                isClosed = false;
                break;
            }
        }
    }

    msg.setService(Services::CHEST_SERVICE);
    msg.setCharacteristic(Characteristics::ARC_REACTOR_CHARACTERISTIC);
    if (isClosed)
    {
        msg.setCommand((uint32_t)FlapsCommands::OPEN);
    }
    else
    {
        msg.setCommand((uint32_t)FlapsCommands::CLOSE);
    }

    std::size_t numBytesSerialized = 0;
    if (msg.serialize(buf, sizeof(buf), numBytesSerialized))
    {
        characteristicRegistry.updateCharacteristic(UUIDs::UUID_CHEST_FLAPS_CHARACTERISTIC, buf, numBytesSerialized);
    }
    else
    {
        // Error
    }
}
