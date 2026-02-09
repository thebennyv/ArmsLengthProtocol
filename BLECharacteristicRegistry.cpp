#include "BLECharacteristicRegistry.h"

#include "UUIDs.h"

// =============================================================================
/* A more detailed version than the header with some internals...
            ┌────────┐                                                          ┌─────────────────────────┐                                            
            │Consumer│                                                          │BLECharacteristicRegistry│                                            
            └───┬────┘                                                          └────────────┬────────────┘                                            
                │                              ┌─────────────────────────────────────────────────────────────────────────────────────┐         
                │                              │Registry will be instantiated as either ArmsLengthServer or ArmsLengthProtoclClient  │         
                │                              └─────────────────────────────────────────────────────────────────────────────────────┘         
                │ attachCharacteristicRegisteredCallback(Consumer::onCharcteristicRegistered)│                                                         
                │ ───────────────────────────────────────────────────────────────────────────>                                                         
                │     attachValueChangedCallback(Consumer::onCharacteristicValueChanged)     │                                                         
                │ ───────────────────────────────────────────────────────────────────────────>                                                         
                │                                                     ╔══════╤═══════════════╪════════════════════════════════════════════════════════╗
                │                                                     ║ ALT  │  Server       │                                                        ║
                │                                                     ╟──────┘               │                                                        ║
                │                                                     ║                      │────┐                                                   ║
                │                                                     ║                      │    │ registerCharacteristic(BLECharacteristic)         ║
                │                                                     ║                      │<───┘                                                   ║
                │                                                     ╠══════════════════════╪════════════════════════════════════════════════════════╣
                │                                                     ║ [Client]             │                                                        ║
                │                                                     ║                      │────┐                                                   ║
                │                                                     ║                      │    │ registerCharacteristic(BLERemoteCharacteristic)   ║
                │                                                     ║                      │<───┘                                                   ║
                │                                                     ╚══════════════════════╪════════════════════════════════════════════════════════╝
                │                                                                            │────┐                                                    
                │                                                                            │    │ characteristicRegistered(BLEUUID)                  
                │                                                                            │<───┘                                                    
                │                  onCharcteristicRegistered(BLEUUID) = true                 │                                                         
                │ <───────────────────────────────────────────────────────────────────────────                                                         
       ┌───────────────────────────────────────────────────────────────────────────────────────────────┐                                               
       │Consumer is interested in this Characteristic, so it returns true.                             │                                               
       │Note that multiple Consumers may be attached, and if *any* of them                             │                                               
       │indicate interest in this Characteristic, then *all* will be notified below                    │                                               
       │when the value changes.                                                                        │                                               
       └───────────────────────────────────────────────────────────────────────────────────────────────┘                                               
                │                                               ┌──────────────────────────────────────────────────────────┐                           
                │                                               │Later, the registered Characteristic's value has changed  │                           
                │                                               └──────────────────────────────────────────────────────────┘                           
   ╔══════╤═════╪════════════════════════════════════════════════════════════════════════════╪════════════════════════════════════════════════════════╗
   ║ ALT  │  Server                                                                          │                                                        ║
   ╟──────┘     │                                                                            │                                                        ║
   ║            │          LocalCharacteristicCallbacks::onWrite()                           │                                                        ║
 ────────────────────────────────────────────────────────────────────────────────────────────>                                                        ║
   ╠════════════╪════════════════════════════════════════════════════════════════════════════╪════════════════════════════════════════════════════════╣
   ║ [Client]   │                                                                            │                                                        ║
   ║            │   global::onRemoteNotification(BLERemoteCharacteristic)                    │                                                        ║
 ────────────────────────────────────────────────────────────────────────────────────────────>                                                        ║
   ║            │                                                                            │────┐                                                   ║
   ║            │                                                                            │    │ RemoteCharacteristicNotifyCallback::onNotify()    ║
   ║            │                                                                            │<───┘                                                   ║
   ║            │                                                                            │────┐                                                   ║
   ║            │                                                                            │    │ dispatchCharacteristicNotification()              ║
   ║            │                                                                            │<───┘                                                   ║
   ╚════════════╪════════════════════════════════════════════════════════════════════════════╪════════════════════════════════════════════════════════╝
                │                                                                            │────┐                                                    
                │                                                                            │    │ valueChanged(BLEUUID, data)                        
                │                                                                            │<───┘                                                    
                │                 onCharacteristicValueChanged(BLEUUID, data)                │                                                         
                │ <───────────────────────────────────────────────────────────────────────────                                                         
       ┌───────────────────────────────────────────────────────────────────────────────────────────────┐                                               
       │Consumer examines the UUID to identify the Characertistic that                                 │                                               
       │changed, and handles the updated data value.                                                   │                                               
       └───────────────────────────────────────────────────────────────────────────────────────────────┘                                               
*/
// =============================================================================

// =============================================================================
// REMOTE UPDATES
// =============================================================================
/*static*/ std::map<BLERemoteCharacteristic*, BLECharacteristicRegistry::RemoteCharacteristicNotifyCallback> BLECharacteristicRegistry::remoteCharacteristicNotifyCallbacks;
void onRemoteNotification(
    BLERemoteCharacteristic *characteristic,
    uint8_t *pData,
    size_t length,
    bool isNotify)
{
    auto mapping = BLECharacteristicRegistry::remoteCharacteristicNotifyCallbacks.find(characteristic);
    if (mapping != BLECharacteristicRegistry::remoteCharacteristicNotifyCallbacks.end())
    {
        mapping->second.onNotify(characteristic, pData, length, isNotify);
    }
    else
    {
        // Not found in map
        Serial.print("Warning: onRemoteNotification() called for unregistered characteristic ");
        Serial.print(characteristic->getUUID().toString().c_str());
        Serial.println(" = ");
        Serial.println(UUIDs::nameOf(characteristic->getUUID()).c_str());
    }
}
BLECharacteristicRegistry::RemoteCharacteristicNotifyCallback::RemoteCharacteristicNotifyCallback():
    m_notifyCallback()
{
}
BLECharacteristicRegistry::RemoteCharacteristicNotifyCallback::RemoteCharacteristicNotifyCallback(
    BLECharacteristicRegistry* client
    ):
    m_notifyCallback(std::bind(
        // Partial function application of function pointer and client object instance
        &BLECharacteristicRegistry::dispatchCharacteristicNotification,
        client,
        // Remaining parameters to be provided when called
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3,
        std::placeholders::_4
        ))
{
}
BLECharacteristicRegistry::RemoteCharacteristicNotifyCallback::RemoteCharacteristicNotifyCallback(const BLECharacteristicRegistry::RemoteCharacteristicNotifyCallback& other):
    m_notifyCallback(other.m_notifyCallback)
{

}
BLECharacteristicRegistry::RemoteCharacteristicNotifyCallback& BLECharacteristicRegistry::RemoteCharacteristicNotifyCallback::operator=(const BLECharacteristicRegistry::RemoteCharacteristicNotifyCallback& other)
{
    if (this != &other)
    {
        m_notifyCallback = other.m_notifyCallback;
    }
    return *this;
}
void BLECharacteristicRegistry::RemoteCharacteristicNotifyCallback::onNotify(
    BLERemoteCharacteristic* characteristic,
    uint8_t* data,
    size_t length,
    bool isNotify)
{
    m_notifyCallback(characteristic, data, length, isNotify); // calls dispatchCharacteristicNotification below
}
void BLECharacteristicRegistry::dispatchCharacteristicNotification(
    BLERemoteCharacteristic *characteristic,
    uint8_t *pData,
    size_t length,
    bool isNotify)
{
    valueChanged(
        characteristic->getUUID(),
        pData,
        length,
        isNotify
        );
}

// =============================================================================
// LOCAL UPDATES
// =============================================================================
BLECharacteristicRegistry::LocalCharacteristicCallbacks::LocalCharacteristicCallbacks(
    BLECharacteristicRegistry* owner
    ):
    pOwner(owner)
{

}
void BLECharacteristicRegistry::LocalCharacteristicCallbacks::onWrite(BLECharacteristic* characteristic) 
{
    pOwner->valueChanged(
        characteristic->getUUID(),
        characteristic->getData(),
        characteristic->getLength(),
        false  // isNotify = false for local writes
        );
}


BLECharacteristicRegistry::BLECharacteristicRegistry():
    m_remoteCharacteristics(),
    m_localCharacteristics(),
    m_characteristicRegisteredCallbacks(),
    m_valueChangedCallbacks()
{
}

BLECharacteristicRegistry::~BLECharacteristicRegistry()
{
}

void BLECharacteristicRegistry::registerCharacteristic(BLECharacteristic* characteristic)
{
    if (characteristicRegistered(characteristic->getUUID()))
    {
        // Set up local characteristic write callback
        characteristic->setCallbacks(new LocalCharacteristicCallbacks(this));
        m_localCharacteristics.push_back(characteristic);
    }
}

void BLECharacteristicRegistry::registerCharacteristic(BLERemoteCharacteristic* characteristic)
{
    if (characteristicRegistered(characteristic->getUUID()))
    {
        // Set up remote characteristic notification callback
        BLECharacteristicRegistry::remoteCharacteristicNotifyCallbacks[characteristic] = this;

        if (characteristic->canNotify())
        {
            characteristic->registerForNotify(onRemoteNotification);
        }
        m_remoteCharacteristics.push_back(characteristic);
    }
}

void BLECharacteristicRegistry::unregisterRemoteCharacteristics()
{
    for (auto& characteristic : m_remoteCharacteristics)
    {
        BLECharacteristicRegistry::remoteCharacteristicNotifyCallbacks.erase(characteristic);
    }
    m_remoteCharacteristics.clear();
}

void BLECharacteristicRegistry::attachCharacteristicRegisteredCallback(characteristicRegistered_callback_t callback)
{
    // MULTITHREADING TODO: On RTOS, lock a mutex here
    m_characteristicRegisteredCallbacks.push_back(callback);
}

void BLECharacteristicRegistry::attachValueChangedCallback(valueChanged_callback_t callback)
{
    // MULTITHREADING TODO: On RTOS, lock a mutex here
    m_valueChangedCallbacks.push_back(callback);
}

void BLECharacteristicRegistry::updateCharacteristic(BLEUUID characteristicUUID, uint8_t* pData, size_t length)
{
    Serial.print( F("BLECharacteristicRegistry::updateCharacteristic length ") );
    Serial.println(length);

    int numLocals = 0;
    int numRemotes = 0;
    for (auto& characteristic : m_localCharacteristics)
    {
        numLocals++;
        
        if (characteristic->getUUID() == characteristicUUID)
        {
            Serial.println( F("characteristic->setValue") );

            characteristic->setValue(pData, length);
            valueChanged(characteristicUUID, pData, length, false /* !notify for local updates */);
            return;
        }
    }
    for (auto& characteristic : m_remoteCharacteristics)
    {
        numRemotes++;
        
        if (characteristic->getUUID() == characteristicUUID)
        {
            Serial.println( F("characteristic->writeValue") );

            characteristic->writeValue(pData, length);
            return;
        }
    }
            Serial.print(F("numLocals = "));
            Serial.print(numLocals);
            Serial.print(F(", numRemotes = "));
            Serial.println(numRemotes);
}

bool BLECharacteristicRegistry::characteristicRegistered(BLEUUID characteristicUUID)
{
    bool retVal = false;
    // Call out to event consumers, who will return true to indicate they want to register for updates to the characteristic.
    for (auto& callback : m_characteristicRegisteredCallbacks)
    {
        if (callback(characteristicUUID))
        {
            retVal = true;
        }
    }
    return retVal;
}

void BLECharacteristicRegistry::valueChanged(
    BLEUUID characteristicUUID,
    uint8_t* pData,
    size_t length,
    bool isNotify)
{
    // Call out to event consumers to handle the updated value.
    for (auto& callback : m_valueChangedCallbacks)
    {
    Serial.print( F("BLECharacteristicRegistry::valueChanged length ") );
    Serial.println(length);

        callback(characteristicUUID, pData, length, isNotify);
    }
}

bool BLECharacteristicRegistry::getValue(
    BLEUUID characteristicUUID,
    uint8_t* pData,
    size_t pDataCapacity,
    size_t& lengthOut)
{
    if (!pData)
    {
        return false;
    }

    for (auto& characteristic : m_localCharacteristics)
    {
        if (characteristic->getUUID() == characteristicUUID)
        {
            lengthOut = characteristic->getLength();
            if (pDataCapacity < lengthOut)
            {
                return false;
            }

            for (size_t i = 0; i < lengthOut; i++)
            {
                pData[i] = characteristic->getData()[i];
            }
            return true;
        }
    }
    for (auto& characteristic : m_remoteCharacteristics)
    {
        if (characteristic->getUUID() == characteristicUUID)
        {
            String val = characteristic->readValue();

            lengthOut = val.length();
            if (pDataCapacity < lengthOut)
            {
                return false;
            }

            for (size_t i = 0; i < lengthOut; i++)
            {
                pData[i] = (uint8_t)val[i];
            }
            return true;
        }
    }
    return false; // not found
}