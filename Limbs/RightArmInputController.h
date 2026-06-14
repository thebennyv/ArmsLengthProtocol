#ifndef RIGHTARMINPUTCONTROLLER_H
#define RIGHTARMINPUTCONTROLLER_H

#include "ArmsLengthController.h"
#include "lib/OneButton/src/OneButton.h"

class RightArmInputController:
    public ArmsLengthController
{
public:

    typedef int NumberOfPresses;
    static const NumberOfPresses LONGPRESS_START = -2;
    static const NumberOfPresses LONGPRESS_STOP = -3;

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
            button.setPressTicks(500);
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
            button.setPressTicks(500);
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

    int flexSensorWatermarkOn;
    int flexSensorWatermarkOff;
    bool flexSenorTriggered;

    void rightArmLaser(LaserCommands command);
    void rightArmRepulsor(RepulsorCommands command);
    void toggleRightArmRocket();
    void dimEyes();
    void toggleFaceplate();
    void toggleEyes();
    void resetReactor();
    void unibeam();
    void toggleShoulderFlaps();
    void toggleAllBackFlaps();
    void partyMode();
    void toggleChestFlaps();

};

#endif // RIGHTARMINPUTCONTROLLER_H
