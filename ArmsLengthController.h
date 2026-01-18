#ifndef ARMLENGTHCONTROLLER_H
#define ARMLENGTHCONTROLLER_H

#include "ArmsLengthProtocol.h"
#include "IArduinoApplication.h"
#include "BLECharacteristicRegistry.h"

class ArmsLengthController:
    public IArduinoApplication
{
public:

    ArmsLengthController(BLECharacteristicRegistry& characteristicRegistry);
    virtual ~ArmsLengthController();

    void setup() override;
    void loop() override;

private:

protected:

    BLECharacteristicRegistry& characteristicRegistry;

};

#endif // ARMLENGTHCONTROLLER_H
