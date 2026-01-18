#ifndef BACK_H
#define BACK_H

#include "IArduinoApplication.h"
#include "BLECharacteristicRegistry.h"

class Back :
    public IArduinoApplication
{
public:
    Back(BLECharacteristicRegistry& characteristicRegistry);
    virtual ~Back();
    
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

#endif // BACK_H
