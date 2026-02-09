#include "ArmsLengthMessageHandler.h"
#include "Arduino.h"

#define ARRAYLEN(arr) sizeof(arr)/sizeof(arr[0])

ArmsLengthMessageHandler::ArmsLengthMessageHandler():
    history()
{
}

ArmsLengthMessageHandler::~ArmsLengthMessageHandler()
{

}

bool ArmsLengthMessageHandler::receiveUniqueMessage(
    ArmsLengthProtocolMessage& msgOut,
    uint8_t* bufferToDeserialize,
    std::size_t bufferSize)
{
    Serial.print( F("ArmsLengthMessageHandler receiveUniqueMessage bufferSize ") );
    Serial.println(bufferSize);

    if (!msgOut.deserialize(bufferToDeserialize, bufferSize))
    {
        Serial.println( F("ArmsLengthMessageHandler deserialize failed") );
        return false;
    }

    // Filter the message against our recent history.
    return history.append(msgOut.getSequenceNumber());
}

ArmsLengthMessageHandler::SequenceNumberHistory::SequenceNumberHistory():
    history(),
    head(0)
{
    for (int i = 0; i < ARRAYLEN(history); i++)
    {
        history[i] = 0; // Init to known invalid value.
    }
}

bool ArmsLengthMessageHandler::SequenceNumberHistory::append(uint32_t sequenceNumber)
{
    for (int i = 0; i < ARRAYLEN(history); i++)
    {
        if (history[i] == sequenceNumber)
        {
        Serial.print( F("SequenceNumberHistory found ") );
        Serial.println(sequenceNumber);
            // Found it in the recent history, so reject it.
            return false;
        }
    }

    // Append sequence number to circular queue, overwriting oldest entry.
    head = (head + 1) % ARRAYLEN(history);
    history[head] = sequenceNumber;
    return true;
}
