#ifndef RIGHTARMINPUTCONTROLLER_H
#define RIGHTARMINPUTCONTROLLER_H

#include "ArmsLengthController.h"
#include "lib/OneButton/src/OneButton.h"

class RightArmInputController:
    public ArmsLengthController
{
public:

    typedef int NumberOfPresses;
    static const NumberOfPresses LONGPRESS = -2;

    struct AttachedButton
    {
        RightArmInputController* controller;
        int pin;
        OneButton button;

        AttachedButton():
            controller(nullptr),
            pin(-1),
            button()
        {
        }

        AttachedButton(
            RightArmInputController* controller,
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

    RightArmInputController(BLECharacteristicRegistry& characteristicRegistry);
    virtual ~RightArmInputController();

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

#endif // RIGHTARMINPUTCONTROLLER_H
