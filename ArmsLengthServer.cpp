#include "ArmsLengthServer.h"

#include "UUIDs.h"

ArmsLengthServer::ArmsLengthServer(
    String deviceName /* e.g. "RightArm" */
    ):
    BLECharacteristicRegistry(),
    m_deviceName(deviceName),
    
    pArmsLengthBLEServer(nullptr),
    
    pScriptService(nullptr),
    pHelmetService(nullptr),
    pChestService(nullptr),
    pBackService(nullptr),
    pRightArmService(nullptr),
    pLeftArmService(nullptr),
    pRightLegService(nullptr),
    pLeftLegService(nullptr),

    pDefaultsScriptCharacteristic(nullptr),
    pThreatDisplayScriptCharacteristic(nullptr),
    pAngryScriptCharacteristic(nullptr),
    pReadyToFlyScriptCharacteristic(nullptr),
    pFlapsTestScriptCharacteristic(nullptr),

    pEyesCharacteristic(nullptr),
    pFaceplateCharacteristic(nullptr),
    pSpeechCharacteristic(nullptr),

    pArcReactorCharacteristic(nullptr),
    pChestFlapsCharacteristic(nullptr),

    pBackFlapsCharacteristic(nullptr),

    pRightArmLaserCharacteristic(nullptr),
    pRightArmRocketCharacteristic(nullptr),
    pRightArmRepulsorCharacteristic(nullptr),

    pLeftArmLaserCharacteristic(nullptr),
    pLeftArmRocketCharacteristic(nullptr),
    pLeftArmRepulsorCharacteristic(nullptr),

    pRightLegFlapsCharacteristic(nullptr),
    pLeftLegFlapsCharacteristic(nullptr),

    numConnectedClients(0)
{
}   

ArmsLengthServer::~ArmsLengthServer()
{
}

void ArmsLengthServer::setup()
{
    String serverName = "ArmsLengthServer " + m_deviceName;

    BLEDevice::init(serverName);

    pArmsLengthBLEServer = BLEDevice::createServer();
    pArmsLengthBLEServer->setCallbacks(new MyServerCallbacks(this));

    // Services
    pScriptService = pArmsLengthBLEServer->createService(UUIDs::UUID_SCRIPT_SERVICE);
    pHelmetService = pArmsLengthBLEServer->createService(UUIDs::UUID_HELMET_SERVICE);
    pChestService = pArmsLengthBLEServer->createService(UUIDs::UUID_CHEST_SERVICE);
    pBackService = pArmsLengthBLEServer->createService(UUIDs::UUID_BACK_SERVICE);
    pRightArmService = pArmsLengthBLEServer->createService(UUIDs::UUID_RIGHT_ARM_SERVICE);
    pLeftArmService = pArmsLengthBLEServer->createService(UUIDs::UUID_LEFT_ARM_SERVICE);
    pRightLegService = pArmsLengthBLEServer->createService(UUIDs::UUID_RIGHT_LEG_SERVICE);
    pLeftLegService = pArmsLengthBLEServer->createService(UUIDs::UUID_LEFT_LEG_SERVICE);

    // Script Service Characteristics
    pDefaultsScriptCharacteristic = pScriptService->createCharacteristic(
        UUIDs::UUID_DEFAULTS_SCRIPT_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
        );
    pThreatDisplayScriptCharacteristic = pScriptService->createCharacteristic(
        UUIDs::UUID_THREAT_DISPLAY_SCRIPT_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
        );
    pAngryScriptCharacteristic = pScriptService->createCharacteristic(
        UUIDs::UUID_ANGRY_SCRIPT_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
        );
    pReadyToFlyScriptCharacteristic = pScriptService->createCharacteristic(
        UUIDs::UUID_READY_TO_FLY_SCRIPT_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
        );
    pFlapsTestScriptCharacteristic = pScriptService->createCharacteristic(
        UUIDs::UUID_FLAPS_TEST_SCRIPT_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
        );

    // Helmet Service Characeristics
    pEyesCharacteristic = pHelmetService->createCharacteristic(
        UUIDs::UUID_EYES_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
        );
    pFaceplateCharacteristic = pHelmetService->createCharacteristic(
        UUIDs::UUID_FACEPLATE_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
        );
    pSpeechCharacteristic = pHelmetService->createCharacteristic(
        UUIDs::UUID_SPEECH_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
        );

    // Chest Service Characteristics
    pArcReactorCharacteristic = pChestService->createCharacteristic(
        UUIDs::UUID_ARC_REACTOR_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
        );
    pChestFlapsCharacteristic = pChestService->createCharacteristic(
        UUIDs::UUID_CHEST_FLAPS_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
        );

    // Back Service Characteristics
    pBackFlapsCharacteristic = pBackService->createCharacteristic(
        UUIDs::UUID_BACK_FLAPS_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
        );

    // Right Arm Service Characteristics
    pRightArmLaserCharacteristic = pRightArmService->createCharacteristic(
        UUIDs::UUID_RIGHT_ARM_LASER_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
        );
    pRightArmRocketCharacteristic = pRightArmService->createCharacteristic(
        UUIDs::UUID_RIGHT_ARM_ROCKET_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
        );
    pRightArmRepulsorCharacteristic = pRightArmService->createCharacteristic(
        UUIDs::UUID_RIGHT_ARM_REPULSOR_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
        );

    // Left Arm Service Characteristics
    pLeftArmLaserCharacteristic = pLeftArmService->createCharacteristic(
        UUIDs::UUID_LEFT_ARM_LASER_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
        );
    pLeftArmRocketCharacteristic = pLeftArmService->createCharacteristic(
        UUIDs::UUID_LEFT_ARM_ROCKET_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
        );
    pLeftArmRepulsorCharacteristic = pLeftArmService->createCharacteristic(
        UUIDs::UUID_LEFT_ARM_REPULSOR_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
        );

    // Right Leg Service Characteristics
    pRightLegFlapsCharacteristic = pRightLegService->createCharacteristic(
        UUIDs::UUID_RIGHT_LEG_FLAPS_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
        );
        
    // Left Leg Service Characteristics
    pLeftLegFlapsCharacteristic = pLeftLegService->createCharacteristic(
        UUIDs::UUID_LEFT_LEG_FLAPS_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
        );

    // Start the services
    pScriptService->start();
    pHelmetService->start();
    pChestService->start();
    pBackService->start();
    pRightArmService->start();
    pLeftArmService->start();
    pRightLegService->start();
    pLeftLegService->start();

    // Announce the characteristics locally
    registerCharacteristic(pDefaultsScriptCharacteristic);
    registerCharacteristic(pThreatDisplayScriptCharacteristic);
    registerCharacteristic(pAngryScriptCharacteristic);
    registerCharacteristic(pReadyToFlyScriptCharacteristic);
    registerCharacteristic(pFlapsTestScriptCharacteristic);
    registerCharacteristic(pEyesCharacteristic);
    registerCharacteristic(pFaceplateCharacteristic);
    registerCharacteristic(pSpeechCharacteristic);
    registerCharacteristic(pArcReactorCharacteristic);
    registerCharacteristic(pChestFlapsCharacteristic);
    registerCharacteristic(pBackFlapsCharacteristic);
    registerCharacteristic(pRightArmLaserCharacteristic);
    registerCharacteristic(pRightArmRocketCharacteristic);
    registerCharacteristic(pRightArmRepulsorCharacteristic);
    registerCharacteristic(pLeftArmLaserCharacteristic);
    registerCharacteristic(pLeftArmRocketCharacteristic);
    registerCharacteristic(pLeftArmRepulsorCharacteristic);
    registerCharacteristic(pRightLegFlapsCharacteristic);
    registerCharacteristic(pLeftLegFlapsCharacteristic);

    // Start advertising
    BLEDevice::getAdvertising()->addServiceUUID(UUIDs::UUID_SCRIPT_SERVICE);
    BLEDevice::getAdvertising()->addServiceUUID(UUIDs::UUID_HELMET_SERVICE);
    BLEDevice::getAdvertising()->addServiceUUID(UUIDs::UUID_CHEST_SERVICE);
    BLEDevice::getAdvertising()->addServiceUUID(UUIDs::UUID_BACK_SERVICE);
    BLEDevice::getAdvertising()->addServiceUUID(UUIDs::UUID_RIGHT_ARM_SERVICE);
    BLEDevice::getAdvertising()->addServiceUUID(UUIDs::UUID_LEFT_ARM_SERVICE);
    BLEDevice::getAdvertising()->addServiceUUID(UUIDs::UUID_RIGHT_LEG_SERVICE);
    BLEDevice::getAdvertising()->addServiceUUID(UUIDs::UUID_LEFT_LEG_SERVICE);
    BLEDevice::getAdvertising()->setScanResponse(true);
    //BLEDevice::getAdvertising()->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
}

void ArmsLengthServer::loop()
{
    // Handle BLE server tasks here
    // ...
}

ArmsLengthServer::MyServerCallbacks::MyServerCallbacks(
    ArmsLengthServer* owner
    ):
    BLEServerCallbacks(),
    pOwner(owner)
{
}

void ArmsLengthServer::MyServerCallbacks::onConnect(BLEServer *pServer)
{
    pOwner->numConnectedClients++;
    pServer->startAdvertising();
}

void ArmsLengthServer::MyServerCallbacks::onDisconnect(BLEServer *pServer)
{
    pOwner->numConnectedClients--;
    pServer->startAdvertising();
}
