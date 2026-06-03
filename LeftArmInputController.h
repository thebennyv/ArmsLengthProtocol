#ifndef LEFTARMINPUTCONTROLLER_H
#define LEFTARMINPUTCONTROLLER_H

#include "ArmsLengthController.h"
#include "lib/OneButton/src/OneButton.h"

class LeftArmInputController:
    public ArmsLengthController
{
public:

    typedef int NumberOfPresses;
    static const NumberOfPresses LONGPRESS_START = -2;
    static const NumberOfPresses LONGPRESS_STOP = -3;

    struct AttachedButton
    {
        LeftArmInputController* controller;
        int pin;
        OneButton button;

        AttachedButton():
            controller(nullptr),
            pin(-1),
            button()
        {
            button.setPressTicks(200);
        }

        AttachedButton(
            LeftArmInputController* controller,
            int pin,
            boolean activeLow,
            bool pullupActive
            ):
            controller(controller),
            pin(pin),
            button(pin, activeLow, pullupActive)
        {
            button.setPressTicks(200);
        }
    };

    LeftArmInputController(BLECharacteristicRegistry& characteristicRegistry);
    virtual ~LeftArmInputController();

    void setup() override;
    void loop() override;

    bool onCharacteristicRegistered(BLEUUID characteristicUUID);

    void buttonPressed(AttachedButton* button, NumberOfPresses numPresses);

private:

protected:

    AttachedButton thumb; // Side of index finger

    int flexSensorWatermarkOn;
    int flexSensorWatermarkOff;
    bool flexSenorTriggered;

    void leftArmLaser(LaserCommands command);
    void leftArmRepulsor(RepulsorCommands command);
    void toggleLeftArmRocket();

};

#endif // LEFTARMINPUTCONTROLLER_H
