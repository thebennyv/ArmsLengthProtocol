#ifndef ARMSLENGTHMESSAGEHANDLER_H
#define ARMSLENGTHMESSAGEHANDLER_H

#include "ArmsLengthProtocolMessage.h"

class ArmsLengthMessageHandler
{
public:

    ArmsLengthMessageHandler();
    virtual ~ArmsLengthMessageHandler();

    bool receiveUniqueMessage(
        ArmsLengthProtocolMessage& msgOut,
        uint8_t* bufferToDeserialize,
        std::size_t bufferSize
        );

protected:

    class SequenceNumberHistory
    {
    public:
        SequenceNumberHistory();

        // Returns true if the submitted sequence number is not already found in
        // the recent history, and the new sequence number has been recorded.
        bool append(uint32_t sequenceNumber);

    private:

        uint32_t history[10];
        int head;
    };
    SequenceNumberHistory history;

};

#endif // ARMSLENGTHMESSAGEHANDLER_H
