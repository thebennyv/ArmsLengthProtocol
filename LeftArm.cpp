#include "LeftArm.h"
#include "UUIDs.h"

LeftArm::LeftArm(BLECharacteristicRegistry& characteristicRegistry):
    characteristicRegistry(characteristicRegistry)
{
    characteristicRegistry.attachCharacteristicRegisteredCallback(
        std::bind(
            &LeftArm::onCharacteristicRegistered,
            this,
            std::placeholders::_1
            )
        );
    characteristicRegistry.attachValueChangedCallback(
        std::bind(
            &LeftArm::onValueChanged,
            this,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4
            )
        );
}

LeftArm::~LeftArm()
{
}   

void LeftArm::setup()
{
}

void LeftArm::loop()
{
}

bool LeftArm::onCharacteristicRegistered(BLEUUID characteristicUUID)
{
    return 
        characteristicUUID.equals(UUIDs::UUID_LEFT_ARM_LASER_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_LEFT_ARM_ROCKET_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_LEFT_ARM_REPULSOR_CHARACTERISTIC);
};

void LeftArm::onValueChanged(
    BLEUUID characteristicUUID,
    uint8_t* pData,
    size_t length,
    bool isNotify)
{
    if (characteristicUUID.equals(UUIDs::UUID_LEFT_ARM_LASER_CHARACTERISTIC))
    {
        // Handle Left Arm Laser Characteristic update
    }
    else if (characteristicUUID.equals(UUIDs::UUID_LEFT_ARM_ROCKET_CHARACTERISTIC))
    {
        // Handle Left Arm Rocket Characteristic update
    }
    else if (characteristicUUID.equals(UUIDs::UUID_LEFT_ARM_REPULSOR_CHARACTERISTIC))
    {
        // Handle Left Arm Repulsor Characteristic update
    }
}
