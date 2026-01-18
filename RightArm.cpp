#include "RightArm.h"
#include "UUIDs.h"

RightArm::RightArm(BLECharacteristicRegistry& characteristicRegistry):
    characteristicRegistry(characteristicRegistry)
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
}

void RightArm::loop()
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
    if (characteristicUUID.equals(UUIDs::UUID_RIGHT_ARM_LASER_CHARACTERISTIC))
    {
        // Handle Right Arm Laser Characteristic update
    }
    else if (characteristicUUID.equals(UUIDs::UUID_RIGHT_ARM_ROCKET_CHARACTERISTIC))
    {
        // Handle Right Arm Rocket Characteristic update
    }
    else if (characteristicUUID.equals(UUIDs::UUID_RIGHT_ARM_REPULSOR_CHARACTERISTIC))
    {
        // Handle Right Arm Repulsor Characteristic update
    }
}
