#ifndef ARMS_LENGTH_PROTOCOL_H
#define ARMS_LENGTH_PROTOCOL_H

#include <cstdint>

enum class Services
{
    SCRIPT_SERVICE = 0,
    HELMET_SERVICE = 1,
    CHEST_SERVICE = 2,
    BACK_SERVICE = 3,
    RIGHT_ARM_SERVICE = 4,
    LEFT_ARM_SERVICE = 5,
    RIGHT_LEG_SERVICE = 6,
    LEFT_LEG_SERVICE = 7
};

enum class Characteristics
{
    // Script Service Characteristics
    DEFAULTS_SCRIPT_CHARACTERISTIC = 0,
    THREAT_DISPLAY_SCRIPT_CHARACTERISTIC = 1,
    ANGRY_SCRIPT_CHARACTERISTIC = 2,
    READY_TO_FLY_SCRIPT_CHARACTERISTIC = 3,
    FLAPS_TEST_SCRIPT_CHARACTERISTIC = 4,

    // Helmet Service Characteristics
    EYES_CHARACTERISTIC = 5,
    FACEPLATE_CHARACTERISTIC = 6,
    SPEECH_CHARACTERISTIC = 7,
    
    // Chest Service Characteristics
    ARC_REACTOR_CHARACTERISTIC = 8,
    CHEST_FLAPS_CHARACTERISTIC = 9,
    
    // Back Service Characteristics
    BACK_FLAPS_CHARACTERISTIC = 10,
    
    // Right Arm Service Characteristics
    RIGHT_ARM_LASER_CHARACTERISTIC = 11,
    RIGHT_ARM_ROCKET_CHARACTERISTIC = 12,
    RIGHT_ARM_REPULSOR_CHARACTERISTIC = 13,
    
    // Left Arm Service Characteristics
    LEFT_ARM_LASER_CHARACTERISTIC = 14,
    LEFT_ARM_ROCKET_CHARACTERISTIC = 15,
    LEFT_ARM_REPULSOR_CHARACTERISTIC = 16,
    
    // Right Leg Service Characteristics
    RIGHT_LEG_FLAPS_CHARACTERISTIC = 17,
    
    // Left Leg Service Characteristics
    LEFT_LEG_FLAPS_CHARACTERISTIC = 18
};

enum class ScriptCommands
{
    RUN = 0
};

enum class EyesCommands
{
    OFF = 0,
    ON = 1,
    FLICKER_ON = 2,
    FLICKER_OFF = 3
};

enum class FaceplateCommands
{
    OPEN = 0,
    CLOSE = 1
};

enum class SpeechCommands
{
    STAND_DOWN = 0,
    GENIUS_BILLIONAIRE = 1,
    I_AM_IRON_MAN = 2,
    NOTHING_WITHOUT_THIS_SUIT = 3,
    DOTH_MOTHER_KNOW = 4,
    BUILT_THIS_IN_A_CAVE = 5,
    PLAYING_GALAGA = 6
};
    
enum class ArcReactorCommands
{
    OFF = 0,
    ON = 1,
    PULSE_ON = 2,
    PULSE_OFF = 3,
    FLASH = 4,
    RAINBOW = 5,
    FLICKER_ON = 6,
    FLICKER_OFF = 7,
    UNIBEAM = 8
};

enum class FlapsCommands
{
    CLOSE = 0,
    OPEN = 1
};

enum class LaserCommands
{
    OFF = 0,
    ON = 1
};

enum class RocketCommands
{
    CLOSE = 0,
    OPEN = 1
};

enum class RepulsorCommands
{
    OFF = 0,
    ON = 1,
    CHARGE = 2,
    FIRE = 3
};

#endif // ARMS_LENGTH_PROTOCOL_H