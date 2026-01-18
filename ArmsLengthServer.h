#ifndef ARMLENGTHSERVER_H
#define ARMLENGTHSERVER_H

// C:\Users\<XYZ>\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.3.5\libraries\BLE\src
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "ArmsLengthProtocol.h"
#include "IArduinoApplication.h"
#include "BLECharacteristicRegistry.h"

class ArmsLengthServer:
    public IArduinoApplication,
    public BLECharacteristicRegistry
{
public:

    ArmsLengthServer(
        String deviceName /* e.g. "RightArm" */
        );
    virtual ~ArmsLengthServer();

    void setup() override;
    void loop() override;

    inline const String& getDeviceName() const
    {
        return m_deviceName;
    }

protected:

    String m_deviceName;
    
// Arm's Length BLE Server
//  * Script Service
//     - Defaults Script Characteristic
//        > Run
//     - Threat Display Script Characteristic
//        > Run
//     - Angry Script Characteristic
//        > Run
//     - Ready to Fly Script Characteristic
//        > Run
//     - Flaps Test Script Characteristic
//        > Run
//  * Helmet Service
//     - Eyes Characteristic
//        > Off
//        > On
//        > Flicker On
//        > Flicker Off
//     - Faceplate Characteristic
//        > Close
//        > Open
//     - Speech Characteristic
//        > Stand Down
//        > Genius billionaire playboy philanthropist
//        > I am Iron Man
//        > If you're nothing without this suit then you shouldn't have it
//        > Doth mother know you weareth her drapes?
//        > I built this in a cave with a box of scraps!
//        > That man is playing Galaga! Thought we wouldn't notice. But we did.
//  * Chest Service
//     - Arc Reactor Characteristic
//        > Off
//        > On
//        > Pulse On
//        > Pulse Off
//        > Flash
//        > Rainbow
//        > Flicker On
//        > Flicker Off
//        > Unibeam
//     - Chest Flaps Characteristic
//        > Close
//        > Open
//  * Back Service
//     - Back Flaps Characteristic
//        > Close
//        > Open
//  * Right Arm Service
//     - Right Arm Laser Characteristic
//        > Off
//        > On
//     - Right Arm Rocket Characteristic
//        > Close
//        > Open
//     - Right Arm Repulsor Characteristic
//        > Off
//        > On
//        > Charge
//        > Fire
//  * Left Arm Service
//     - Left Arm Laser Characteristic
//        > Off
//        > On
//     - Left Arm Rocket Characteristic
//        > Close
//        > Open
//     - Left Arm Repulsor Characteristic
//        > Off
//        > On
//        > Charge
//        > Fire
//  * Right Leg Service
//     - Right Leg Flaps Characteristic
//        > Close
//        > Open
//  * Left Leg Service
//     - Left Leg Flaps Characteristic
//        > Close
//        > Open


    BLEServer* pArmsLengthBLEServer;
    
    BLEService* pScriptService;
    BLEService* pHelmetService;
    BLEService* pChestService;
    BLEService* pBackService;
    BLEService* pRightArmService;
    BLEService* pLeftArmService;
    BLEService* pRightLegService;
    BLEService* pLeftLegService;

    BLECharacteristic* pDefaultsScriptCharacteristic;
    BLECharacteristic* pThreatDisplayScriptCharacteristic;
    BLECharacteristic* pAngryScriptCharacteristic;
    BLECharacteristic* pReadyToFlyScriptCharacteristic;
    BLECharacteristic* pFlapsTestScriptCharacteristic;

    BLECharacteristic* pEyesCharacteristic;
    BLECharacteristic* pFaceplateCharacteristic;
    BLECharacteristic* pSpeechCharacteristic;

    BLECharacteristic* pArcReactorCharacteristic;
    BLECharacteristic* pChestFlapsCharacteristic;

    BLECharacteristic* pBackFlapsCharacteristic;

    BLECharacteristic* pRightArmLaserCharacteristic;
    BLECharacteristic* pRightArmRocketCharacteristic;
    BLECharacteristic* pRightArmRepulsorCharacteristic;

    BLECharacteristic* pLeftArmLaserCharacteristic;
    BLECharacteristic* pLeftArmRocketCharacteristic;
    BLECharacteristic* pLeftArmRepulsorCharacteristic;

    BLECharacteristic* pRightLegFlapsCharacteristic;

    BLECharacteristic* pLeftLegFlapsCharacteristic;

    
    class MyServerCallbacks : public BLEServerCallbacks
    {
    public:
        MyServerCallbacks(ArmsLengthServer* owner);
        
        void onConnect(BLEServer *pServer) override;
        void onDisconnect(BLEServer *pServer) override;

    protected:
        ArmsLengthServer* pOwner;
    };
    friend class MyServerCallbacks;


    int numConnectedClients;
};


#endif // ARMLENGTHSERVER_H
