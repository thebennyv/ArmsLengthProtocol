#ifndef RIGHT_ARM_H
#define RIGHT_ARM_H

#include "IArduinoApplication.h"
#include "BLECharacteristicRegistry.h"

class RightArm :
    public IArduinoApplication
{
public:
    RightArm(BLECharacteristicRegistry& characteristicRegistry);
    virtual ~RightArm();
    
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

#endif // RIGHT_ARM_H
