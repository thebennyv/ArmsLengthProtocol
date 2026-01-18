#include "Chest.h"
#include "UUIDs.h"

Chest::Chest(BLECharacteristicRegistry& characteristicRegistry):
    characteristicRegistry(characteristicRegistry)
{
    characteristicRegistry.attachCharacteristicRegisteredCallback(
        std::bind(
            &Chest::onCharacteristicRegistered,
            this,
            std::placeholders::_1
            )
        );
    characteristicRegistry.attachValueChangedCallback(
        std::bind(
            &Chest::onValueChanged,
            this,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4
            )
        );
}

Chest::~Chest()
{
}   

void Chest::setup()
{
}

void Chest::loop()
{
}

bool Chest::onCharacteristicRegistered(BLEUUID characteristicUUID)
{
    return 
        characteristicUUID.equals(UUIDs::UUID_ARC_REACTOR_CHARACTERISTIC) ||
        characteristicUUID.equals(UUIDs::UUID_CHEST_FLAPS_CHARACTERISTIC);
};

void Chest::onValueChanged(
    BLEUUID characteristicUUID,
    uint8_t* pData,
    size_t length,
    bool isNotify)
{
    if (characteristicUUID.equals(UUIDs::UUID_ARC_REACTOR_CHARACTERISTIC))
    {
        // Handle Arc Reactor Characteristic update
    }
    else if (characteristicUUID.equals(UUIDs::UUID_CHEST_FLAPS_CHARACTERISTIC))
    {
        // Handle Chest Flaps Characteristic update
    }
}
