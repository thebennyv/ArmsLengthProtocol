#ifndef CHEST_H
#define CHEST_H

#include "IArduinoApplication.h"
#include "BLECharacteristicRegistry.h"

class Chest :
    public IArduinoApplication
{
public:
    Chest(BLECharacteristicRegistry& characteristicRegistry);
    virtual ~Chest();
    
    void setup() override;
    void loop() override;

    bool onCharacteristicRegistered(BLEUUID characteristicUUID);

    void onValueChanged(
        BLEUUID characteristicUUID,
        uint8_t* pData,
        size_t length,
        bool isNotify
        );

private:

    BLECharacteristicRegistry& characteristicRegistry;

protected:
};

#endif // CHEST_H
