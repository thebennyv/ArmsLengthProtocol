#ifndef HELMET_H
#define HELMET_H

#include "IArduinoApplication.h"
#include "BLECharacteristicRegistry.h"

class Helmet :
    public IArduinoApplication
{
public:
    Helmet(BLECharacteristicRegistry& characteristicRegistry);
    virtual ~Helmet();
    
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

#endif // HELMET_H
