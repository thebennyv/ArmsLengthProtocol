#include "Back.h"
#include "UUIDs.h"

Back::Back(BLECharacteristicRegistry& characteristicRegistry):
    characteristicRegistry(characteristicRegistry)
{
    characteristicRegistry.attachCharacteristicRegisteredCallback(
        std::bind(
            &Back::onCharacteristicRegistered,
            this,
            std::placeholders::_1
            )
        );
    characteristicRegistry.attachValueChangedCallback(
        std::bind(
            &Back::onValueChanged,
            this,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4
            )
        );
}

Back::~Back()
{
}

void Back::setup()
{
}

void Back::loop()
{
}

bool Back::onCharacteristicRegistered(BLEUUID characteristicUUID)
{
    return characteristicUUID.equals(UUIDs::UUID_BACK_FLAPS_CHARACTERISTIC);
};

void Back::onValueChanged(
    BLEUUID characteristicUUID,
    uint8_t* pData,
    size_t length,
    bool isNotify)
{
    if (characteristicUUID.equals(UUIDs::UUID_BACK_FLAPS_CHARACTERISTIC))
    {
        // Handle Back Flaps Characteristic update
    }
}
