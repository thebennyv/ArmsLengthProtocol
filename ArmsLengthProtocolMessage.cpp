#include "ArmsLengthProtocolMessage.h"

#include <cstring>
#include "Endian.h"

ArmsLengthProtocolMessage::ArmsLengthProtocolMessage(
    Services service,
    Characteristics characteristic,
    uint32_t command
    ):
    m_sequenceNumber(0), // set during serialization/deserialization
    m_service(static_cast<uint32_t>(service)),
    m_characteristic(static_cast<uint32_t>(characteristic)),
    m_command(command)
{
}

ArmsLengthProtocolMessage::~ArmsLengthProtocolMessage()
{
}

uint32_t ArmsLengthProtocolMessage::getSequenceNumber() const
{
    return m_sequenceNumber;
}

void ArmsLengthProtocolMessage::setService(Services service)
{
    m_service = static_cast<uint32_t>(service);
}
Services ArmsLengthProtocolMessage::getService() const
{
    return static_cast<Services>(m_service);
}

void ArmsLengthProtocolMessage::setCharacteristic(Characteristics characteristic)
{
    m_characteristic = static_cast<uint32_t>(characteristic);
}
Characteristics ArmsLengthProtocolMessage::getCharacteristic() const
{
    return static_cast<Characteristics>(m_characteristic);
}

void ArmsLengthProtocolMessage::setCommand(uint32_t command)
{
    m_command = command;
}
uint32_t ArmsLengthProtocolMessage::getCommand() const
{
    return m_command;
}

bool ArmsLengthProtocolMessage::serialize(
    uint8_t* buffer,
    size_t bufferSize,
    size_t& bytesWritten)
{
    const size_t requiredSize = sizeof(m_sequenceNumber) + sizeof(m_service) + sizeof(m_characteristic) + sizeof(m_command);
    if (bufferSize < requiredSize)
    {
        bytesWritten = 0;
        return false;
    }

    size_t offset = 0;

    uint32_t tempU32 = 0;

    m_sequenceNumber = getNextSequenceNumber(); // new sequence number upon serialization

    tempU32 = toBigEndian<uint32_t>(m_sequenceNumber);
    std::memcpy(buffer + offset, &tempU32, sizeof(tempU32));
    offset += sizeof(tempU32);

    tempU32 = toBigEndian<uint32_t>(m_service);
    std::memcpy(buffer + offset, &tempU32, sizeof(tempU32));
    offset += sizeof(tempU32);

    tempU32 = toBigEndian<uint32_t>(m_characteristic);
    std::memcpy(buffer + offset, &tempU32, sizeof(tempU32));
    offset += sizeof(tempU32);

    tempU32 = toBigEndian<uint32_t>(m_command);
    std::memcpy(buffer + offset, &tempU32, sizeof(tempU32));
    offset += sizeof(tempU32);

    bytesWritten = offset;
    return true;
}

bool ArmsLengthProtocolMessage::deserialize(
    const uint8_t* buffer,
    size_t bufferSize)
{
    const size_t requiredSize = sizeof(m_sequenceNumber) + sizeof(m_service) + sizeof(m_characteristic) + sizeof(m_command);
    if (bufferSize < requiredSize)
    {
        return false;
    }

    size_t offset = 0;

    uint32_t tempU32 = 0;

    std::memcpy(&tempU32, buffer + offset, sizeof(tempU32));
    m_sequenceNumber = fromBigEndian<uint32_t>(tempU32);
    offset += sizeof(tempU32);

    std::memcpy(&tempU32, buffer + offset, sizeof(tempU32));
    m_service = fromBigEndian<uint32_t>(tempU32);
    offset += sizeof(tempU32);

    std::memcpy(&tempU32, buffer + offset, sizeof(tempU32));
    m_characteristic = fromBigEndian<uint32_t>(tempU32);
    offset += sizeof(tempU32);

    std::memcpy(&tempU32, buffer + offset, sizeof(tempU32));
    m_command = fromBigEndian<uint32_t>(tempU32);
    offset += sizeof(tempU32);

    return true;
}