#ifndef ARMSLENGTHPROTOCOLMESSAGE_H
#define ARMSLENGTHPROTOCOLMESSAGE_H

#include <cstdint>
#include "ArmsLengthProtocol.h"

class ArmsLengthProtocolMessage
{
public:

    ArmsLengthProtocolMessage(
        Services service,
        Characteristics characteristic,
        uint32_t command
        );
    virtual ~ArmsLengthProtocolMessage();

    uint32_t getSequenceNumber() const;

    void setService(Services service);
    Services getService() const;

    void setCharacteristic(Characteristics characteristic);
    Characteristics getCharacteristic() const;

    void setCommand(uint32_t command);
    uint32_t getCommand() const;

    bool serialize(
        uint8_t* buffer,
        std::size_t bufferSize,
        std::size_t& bytesWritten
        );
        
    bool deserialize(
        const uint8_t* buffer,
        std::size_t bufferSize
        );

protected:

    uint32_t m_sequenceNumber;
    uint32_t m_service; // See Services enum
    uint32_t m_characteristic; // See Characteristics enum
    uint32_t m_command; // See ScriptCommands, EyesCommands, etc.

    static uint32_t getNextSequenceNumber()
    {
        static uint32_t sequenceNumber = 0;
        return ++sequenceNumber;
    }

};

#endif // ARMSLENGTHPROTOCOLMESSAGE_H
