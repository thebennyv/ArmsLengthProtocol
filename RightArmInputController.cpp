
#include "RightArmInputController.h"
#include "UUIDs.h"

RightArmInputController::RightArmInputController(
    BLECharacteristicRegistry& characteristicRegistry
    ):
    ArmsLengthController(characteristicRegistry)
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
}

void RightArmInputController::loop()
{
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
