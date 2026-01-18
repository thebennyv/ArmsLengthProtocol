#ifndef RIGHTARMINPUTCONTROLLER_H
#define RIGHTARMINPUTCONTROLLER_H

#include "ArmsLengthController.h"

class RightArmInputController:
    public ArmsLengthController
{
public:

    RightArmInputController(BLECharacteristicRegistry& characteristicRegistry);
    virtual ~RightArmInputController();

    void setup() override;
    void loop() override;

    bool onCharacteristicRegistered(BLEUUID characteristicUUID);

private:

protected:

};

#endif // RIGHTARMINPUTCONTROLLER_H
