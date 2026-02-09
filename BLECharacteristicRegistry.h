#ifndef BLECHARACTERISTICREGISTRY_H
#define BLECHARACTERISTICREGISTRY_H

#include <vector>
#include <functional>
#include <BLERemoteCharacteristic.h>
#include <BLECharacteristic.h>

// =============================================================================
// This is the device's local hub for Characteristics.
// Consumer class instances attach callback functions to their local
// BLECharacteristicRegistry instance (Server or Client) to be notified when
// Characteristics are registered and when their values have changed.
// As a special case, "controller" instances will register for any
// Characteristics they need to control/update, but will not register a handler
// for data value updates. Controllers use this registry to update Characteristics,
// which propagates both locally and remotely via BLE.
/*
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
class BLECharacteristicRegistry
{
public:

    BLECharacteristicRegistry();
    virtual ~BLECharacteristicRegistry();

    // =========================================================================
    // Function signatures for Consumer callbacks...
    // =========================================================================

    typedef std::function<
        // Consumer must return true if they are interested in the announced Characteristic.
        bool(BLEUUID /*characteristicUUID*/)
    > characteristicRegistered_callback_t;

    typedef std::function<
        void(BLEUUID /*characteristicUUID*/, uint8_t* /*pData*/, size_t /*length*/, bool /*isNotify*/)
    > valueChanged_callback_t;

    // =========================================================================
    // Consumers call these functions to attach their callbacks...
    // =========================================================================

    // Attached callback will be executed when any Characteristic is announced.
    // When the callback is executed, 
    void attachCharacteristicRegisteredCallback(characteristicRegistered_callback_t callback);

    // Attached callback will be executed when any Characteristic's data value has changed.
    void attachValueChangedCallback(valueChanged_callback_t callback);

    // =========================================================================
    // Controllers call this function to update the value of a Characteristic.
    // =========================================================================
    void updateCharacteristic(BLEUUID characteristicUUID, uint8_t* pData, size_t length);

    // When a Characteristic's data value has changed, this is called to execute
    // all Consumer callbacks to handle the updated value.
    // TODO: Ideally, this would be private, but I'm not sure the BLEServer
    //       will produce BLECharacteristicCallbacks when the local server side
    //       BLECharacteristics are updated directly. This is public for the
    //       moment in case we need to explicitly call it from the server's
    //       local controller(s).
    void valueChanged(
        BLEUUID characteristicUUID,
        uint8_t* pData,
        size_t length,
        bool isNotify
        );

    bool getValue(
        BLEUUID characteristicUUID,
        uint8_t* pData,
        size_t pDataCapacity,
        size_t& lengthOut
        );

protected:

    // Inheritors (Server and Client respectively) call these to announce the
    // presence of a Characteristic.
    void registerCharacteristic(BLECharacteristic* characteristic);
    void registerCharacteristic(BLERemoteCharacteristic* characteristic);

    // Typically called by a BLE Client when it disconnects and loses access to
    // the server's remote Characteristics.
    void unregisterRemoteCharacteristics();

private:

    // =========================================================================
    // REMOTE UPDATES
    // =========================================================================
    // Remote characteristic notification via BLE
    //   --> ::onRemoteNotification() global function {remoteCharacteristicNotifyCallbacks map lookup}
    //     --> RemoteCharacteristicNotifyCallback::onNotify()
    //       --> client instance dispatchCharacteristicNotification()
    //         --> valueChanged() { consumer handles via UUID lookup }
    class RemoteCharacteristicNotifyCallback
    {
    public:

        RemoteCharacteristicNotifyCallback();
        RemoteCharacteristicNotifyCallback(BLECharacteristicRegistry* client);
        RemoteCharacteristicNotifyCallback(const RemoteCharacteristicNotifyCallback& other);

        RemoteCharacteristicNotifyCallback& operator=(const RemoteCharacteristicNotifyCallback& other);

        void onNotify(BLERemoteCharacteristic* characteristic, uint8_t* data, size_t length, bool isNotify);

    private:

        std::function<void(BLERemoteCharacteristic*, uint8_t*, size_t, bool)> m_notifyCallback;
    };
    friend class RemoteCharacteristicNotifyCallback;
    static std::map<BLERemoteCharacteristic*, RemoteCharacteristicNotifyCallback> remoteCharacteristicNotifyCallbacks;
    std::vector<BLERemoteCharacteristic*> m_remoteCharacteristics;
    friend void onRemoteNotification(BLERemoteCharacteristic *characteristic, uint8_t *pData, size_t length, bool isNotify);
    void dispatchCharacteristicNotification(BLERemoteCharacteristic *characteristic, uint8_t *pData, size_t length, bool isNotify);

    // =========================================================================
    // LOCAL UPDATES
    // =========================================================================
    class LocalCharacteristicCallbacks : public BLECharacteristicCallbacks
    {
    public:

        LocalCharacteristicCallbacks(BLECharacteristicRegistry* owner);

        void onWrite(BLECharacteristic* characteristic) override;

    protected:

        BLECharacteristicRegistry* pOwner;
    };
    friend class MyCharacteristicCallbacks;
    std::vector<BLECharacteristic*> m_localCharacteristics;

    // Announce the presence of a Characteristic to Consumers, who will return
    // true to indicate they want to register for updates to the Characteristic.
    bool characteristicRegistered(BLEUUID characteristicUUID);

    // =========================================================================
    // Attached Consumer callbacks...
    // =========================================================================
    std::vector<characteristicRegistered_callback_t> m_characteristicRegisteredCallbacks;
    std::vector<valueChanged_callback_t> m_valueChangedCallbacks;

};

#endif // BLECHARACTERISTICREGISTRY_H
