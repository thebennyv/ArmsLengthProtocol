#ifndef LEFT_LEG_H
#define LEFT_LEG_H

#include "IArduinoApplication.h"
#include "BLECharacteristicRegistry.h"

class LeftLeg :
    public IArduinoApplication
{
public:
    LeftLeg(BLECharacteristicRegistry& characteristicRegistry);
    virtual ~LeftLeg();
    
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

#endif // LEFT_LEG_H
