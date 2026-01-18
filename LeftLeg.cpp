#include "LeftLeg.h"
#include "UUIDs.h"

LeftLeg::LeftLeg(BLECharacteristicRegistry& characteristicRegistry):
    characteristicRegistry(characteristicRegistry)
{
    characteristicRegistry.attachCharacteristicRegisteredCallback(
        std::bind(
            &LeftLeg::onCharacteristicRegistered,
            this,
            std::placeholders::_1
            )
        );
    characteristicRegistry.attachValueChangedCallback(
        std::bind(
            &LeftLeg::onValueChanged,
            this,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4
            )
        );
}

LeftLeg::~LeftLeg()
{
}

void LeftLeg::setup()
{
}

void LeftLeg::loop()
{
}

bool LeftLeg::onCharacteristicRegistered(BLEUUID characteristicUUID)
{
    return characteristicUUID.equals(UUIDs::UUID_LEFT_LEG_FLAPS_CHARACTERISTIC);
};

void LeftLeg::onValueChanged(
    BLEUUID characteristicUUID,
    uint8_t* pData,
    size_t length,
    bool isNotify)
{
    if (characteristicUUID.equals(UUIDs::UUID_LEFT_LEG_FLAPS_CHARACTERISTIC))
    {
        // Handle Left Leg Flaps Characteristic update
    }
}
