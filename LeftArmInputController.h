#ifndef LEFTARMINPUTCONTROLLER_H
#define LEFTARMINPUTCONTROLLER_H

#include "ArmsLengthController.h"
#include "lib/OneButton/src/OneButton.h"

class LeftArmInputController:
    public ArmsLengthController
{
public:

    typedef int NumberOfPresses;
    static const NumberOfPresses LONGPRESS = -2;

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
    AttachedButton indexFinger; // Tip of index finger
    AttachedButton middleFinger; // Tip of middle finger
    AttachedButton ringFinger; // Tip of ring finger
    AttachedButton pinkyFinger; // Tip of pinky finger

    void toggleRightArmRocket();
    void toggleLeftArmRocket();

};

#endif // LEFTARMINPUTCONTROLLER_H
