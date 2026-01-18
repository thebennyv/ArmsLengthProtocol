#ifndef ARMLENGTHCLIENT_H
#define ARMLENGTHCLIENT_H

// C:\Users\<XYZ>\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.3.5\libraries\BLE\src
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "ArmsLengthProtocol.h"
#include "IArduinoApplication.h"
#include "BLECharacteristicRegistry.h"

class ArmsLengthClient:
    public IArduinoApplication,
    public BLECharacteristicRegistry
{
public:

    ArmsLengthClient(
        String deviceName /* e.g. "LeftArm", "Helmet", "iPhone" */
        );
    virtual ~ArmsLengthClient();

    void setup() override;
    void loop() override;

    virtual bool connectToServer();
    virtual void disconnectFromServer();

    inline const String& getDeviceName() const
    {
        return m_deviceName;
    }

private:

    void setBLEClient(BLEClient *client);
    void setServerDevice(BLEAdvertisedDevice *serverDevice);

    String m_deviceName;
    
    BLEClient *pArmsLengthBLEClient;
    BLEAdvertisedDevice *pServerDevice;

protected:

    class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
    {
    public:

        MyAdvertisedDeviceCallbacks(ArmsLengthClient *owner);

        void onResult(BLEAdvertisedDevice advertisedDevice) override;

    private:

        ArmsLengthClient *pOwner;
    };

    class MyClientCallbacks : public BLEClientCallbacks
    {
    public:

        MyClientCallbacks(ArmsLengthClient *owner);

        void onConnect(BLEClient *pClient) override;

        void onDisconnect(BLEClient *pClient) override;

    private:

        ArmsLengthClient *pOwner;
    };

    void setAdvertisedDeviceCallbacks(MyAdvertisedDeviceCallbacks *advertisedDeviceCallbacks);
    void setClientCallbacks(MyClientCallbacks *clientCallbacks);

    // Called when a device is found during scanning. We are looking for the server device.
    MyAdvertisedDeviceCallbacks *pAdvertisedDeviceCallbacks;

    MyClientCallbacks *pClientCallbacks;

};

#endif // ARMLENGTHCLIENT_H
