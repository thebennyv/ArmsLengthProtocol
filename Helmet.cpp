#include "Helmet.h"
#include "UUIDs.h"

Helmet::Helmet(BLECharacteristicRegistry& characteristicRegistry):
    characteristicRegistry(characteristicRegistry)
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
}

void Helmet::loop()
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
    if (characteristicUUID.equals(UUIDs::UUID_EYES_CHARACTERISTIC))
    {
        // Handle Eyes Characteristic update
    }
    else if (characteristicUUID.equals(UUIDs::UUID_FACEPLATE_CHARACTERISTIC))
    {
        // Handle Faceplate Characteristic update
    }
    else if (characteristicUUID.equals(UUIDs::UUID_SPEECH_CHARACTERISTIC))
    {
        // Handle Speech Characteristic update
    }
}
