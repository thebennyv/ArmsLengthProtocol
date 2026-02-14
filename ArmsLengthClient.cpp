
#include "ArmsLengthClient.h"
#include "UUIDs.h"

ArmsLengthClient::ArmsLengthClient(
    String deviceName
    ):
    BLECharacteristicRegistry(),
    m_deviceName(deviceName),
    pArmsLengthBLEClient(nullptr),
    pServerDevice(nullptr),
    pAdvertisedDeviceCallbacks(nullptr),
    pClientCallbacks(nullptr)
{
    setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(this));
    setClientCallbacks(new MyClientCallbacks(this));
}

ArmsLengthClient::~ArmsLengthClient()
{
    setBLEClient(nullptr);
    setServerDevice(nullptr);
    setAdvertisedDeviceCallbacks(nullptr);
    setClientCallbacks(nullptr);
}

void ArmsLengthClient::setBLEClient(BLEClient *client)
{
    if (pArmsLengthBLEClient)
    {
        delete pArmsLengthBLEClient;
        pArmsLengthBLEClient = nullptr;
    }

    pArmsLengthBLEClient = client;
}

void ArmsLengthClient::setServerDevice(BLEAdvertisedDevice *serverDevice)
{
    if (pServerDevice)
    {
        delete pServerDevice;
        pServerDevice = nullptr;
    }

    pServerDevice = serverDevice;
}

void ArmsLengthClient::setAdvertisedDeviceCallbacks(MyAdvertisedDeviceCallbacks *advertisedDeviceCallbacks)
{
    if (pAdvertisedDeviceCallbacks)
    {
        delete pAdvertisedDeviceCallbacks;
        pAdvertisedDeviceCallbacks = nullptr;
    }

    pAdvertisedDeviceCallbacks = advertisedDeviceCallbacks;
}

void ArmsLengthClient::setClientCallbacks(MyClientCallbacks *clientCallbacks)
{
    if (pClientCallbacks)
    {
        delete pClientCallbacks;
        pClientCallbacks = nullptr;
    }

    pClientCallbacks = clientCallbacks;
}

ArmsLengthClient::MyAdvertisedDeviceCallbacks::MyAdvertisedDeviceCallbacks(
    ArmsLengthClient *owner
    ):
    BLEAdvertisedDeviceCallbacks(),
    pOwner(owner)
{
}

void ArmsLengthClient::MyAdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice)
{
    Serial.println("advertisedDevice '" + advertisedDevice.getName() + "'");

    // Could instead query it for a specific service UUID, but this is generic
    // and positions us to have multiple numbered servers (multiple suit instances).
    if (advertisedDevice.getName().indexOf("Arms" /*"ArmsLengthServer"*/) != -1)
    {
        pOwner->setServerDevice(new BLEAdvertisedDevice(advertisedDevice));

        // Stop the scan early, since we've found our server.
        BLEDevice::getScan()->stop();
    }
}

ArmsLengthClient::MyClientCallbacks::MyClientCallbacks(
    ArmsLengthClient *owner
    ):
    BLEClientCallbacks(),
    pOwner(owner)
{
}

void ArmsLengthClient::MyClientCallbacks::onConnect(BLEClient *pClient)
{
    
}

void ArmsLengthClient::MyClientCallbacks::onDisconnect(BLEClient *pClient)
{
    pOwner->unregisterRemoteCharacteristics();

    pOwner->setServerDevice(nullptr);

    pClient->disconnect(); // Google AI says memory leaks abound if we don't do this.
}

void ArmsLengthClient::setup()
{
    BLEDevice::init(m_deviceName);

    setBLEClient(BLEDevice::createClient());

    // https://electronics.stackexchange.com/a/359158
    // only one advertising channel is being looked at during each scanInterval and is rotated to the next of 3 channels with each interval
    // apparently, Apple uses 30 ms scanWindow with 40 ms scanInterval

    //   |------------scanInterval------------|------------scanInterval------------|------------scanInterval------------|
    //   |-------scanWindow-------|           |-------scanWindow-------|           |-------scanWindow-------|
    //         scan channel 37                      scan channel 38                      scan channel 39

    // Set up scan parameters and start scanning
    BLEDevice::getScan()->setAdvertisedDeviceCallbacks(pAdvertisedDeviceCallbacks);
    BLEDevice::getScan()->setInterval(40); // 40 milliseconds (examples used 1349 ms)
    BLEDevice::getScan()->setWindow(30); // 30 milliseconds (examples used 449 ms)
    BLEDevice::getScan()->setActiveScan(true);
    BLEDevice::getScan()->start(30 /* seconds */, nullptr); // non-blocking version

    Serial.println("BLEDevice started scan");
}

void ArmsLengthClient::loop()
{
    // If we have found the server device, but aren't connected yet.
    if (pServerDevice != nullptr && !pArmsLengthBLEClient->isConnected())
    {
        connectToServer();
    }

    // Restart the scan if we haven't found the server or have been disconnected.
    if (pServerDevice == nullptr
     && !BLEDevice::getScan()->isScanning())
    {
        BLEDevice::getScan()->start(5 /* seconds */, nullptr); // non-blocking version

        // TODO: should we switch to a passive scan and different interval/windw?
    }
}

bool ArmsLengthClient::connectToServer()
{
    // Connect to the remote BLE Server.
    if (pArmsLengthBLEClient->connect(pServerDevice))  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    {
        Serial.println("Connected to server");

        pArmsLengthBLEClient->setMTU(517);  //set client to request maximum MTU from server (default is 23 otherwise)

        for (auto const& servicePair : *(pArmsLengthBLEClient->getServices()))
        {
            BLERemoteService* service = servicePair.second;
            Serial.print("Found service: ");
            Serial.print(service->getUUID().toString().c_str());
            Serial.print(" = ");
            Serial.println(UUIDs::nameOf(service->getUUID()).c_str());

            auto characteristics = service->getCharacteristics();
            for (auto const& charPair : *characteristics)
            {
                BLERemoteCharacteristic* characteristic = charPair.second;
                Serial.print("  Found characteristic: ");
                Serial.print(characteristic->getUUID().toString().c_str());
                Serial.print(" = ");
                Serial.println(UUIDs::nameOf(characteristic->getUUID()).c_str());

                registerCharacteristic(characteristic);
            }
        }

        return true;
    }
    else
    {
        Serial.println("Failed to connect to server");
        return false;
    }
}

void ArmsLengthClient::disconnectFromServer()
{
    // Implementation for disconnecting from the server
}
