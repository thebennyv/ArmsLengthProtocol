#ifndef ARMLENGTHSCRIPTS_H
#define ARMLENGTHSCRIPTS_H

#include "ArmsLengthProtocol.h"
#include "IArduinoApplication.h"
#include "BLECharacteristicRegistry.h"

class ArmsLengthScripts:
    public IArduinoApplication
{
public:

    ArmsLengthScripts(BLECharacteristicRegistry& characteristicRegistry);
    virtual ~ArmsLengthScripts();

    void setup() override;
    void loop() override;


private:

    BLECharacteristicRegistry& characteristicRegistry;

protected:

};

#endif // ARMLENGTHSCRIPTS_H
