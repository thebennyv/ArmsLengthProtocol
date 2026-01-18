#ifndef RIGHT_LEG_H
#define RIGHT_LEG_H

#include "IArduinoApplication.h"
#include "BLECharacteristicRegistry.h"

class RightLeg :
    public IArduinoApplication
{
public:
    RightLeg(BLECharacteristicRegistry& characteristicRegistry);
    virtual ~RightLeg();
    
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

#endif // RIGHT_LEG_H
