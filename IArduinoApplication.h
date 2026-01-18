#ifndef IARDUINOAPPLICATION_H
#define IARDUINOAPPLICATION_H

// =============================================================================
// Abstract base class / Interface for objects that should perform activities
// according to the setup+loop Arduino framework.
// =============================================================================
class IArduinoApplication
{
public:

    virtual void setup() = 0;
    virtual void loop() = 0;
};

#endif // IARDUINOAPPLICATION_H
