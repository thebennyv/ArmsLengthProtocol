#ifndef LEFT_ARM_H
#define LEFT_ARM_H

#include "IArduinoApplication.h"
#include "BLECharacteristicRegistry.h"

class LeftArm :
    public IArduinoApplication
{
public:
    LeftArm(BLECharacteristicRegistry& characteristicRegistry);
    virtual ~LeftArm();
    
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

#endif // LEFT_ARM_H
