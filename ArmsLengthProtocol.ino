// =============================================================================
// Arm's Length Protocol
//
// Description:
//   An Iron Man suit controller built around Arduino, Bluetooth Low Energy,
//   and the Xiao ESP32 C6 microcontroller.
//   This project is inspired by https://github.com/crashworks3d/Iron_Man_Servo,
//   but uses none of its code.
//
// Dependencies:
//   Arduino Libraries:
//     OneButton by Matthias Hertel v2.6.1
//     ESP32Servo by Kevin Harrington, John K. Bennett v3.2.0 (modified)
//       (with ESP32PWM)
//     FastLED by Daniel Garcia v3.10.3
//     Adafruit Neopixel by Adafruit v1.15.5
//     Protothreads by Ben Artin, Adam Dunkels v1.4.0-arduino.beta.1
//     DFPlayerMini by Makuna v1.0.7
//     ESPSoftwareSerial by Dirk Kaar, Peter Lerup v8.1.0s
// =============================================================================

#include <Arduino.h>
#include "ArmsLengthProtocol.h"

#define LIMB_HELMET 1
#define LIMB_CHEST 2
#define LIMB_BACK 3
#define LIMB_RIGHT_ARM 4
#define LIMB_LEFT_ARM 5
#define LIMB_RIGHT_LEG 6
#define LIMB_LEFT_LEG 7

#define LIMB LIMB_HELMET
//#define LIMB LIMB_CHEST
//#define LIMB LIMB_BACK
//#define LIMB LIMB_RIGHT_ARM
//#define LIMB LIMB_LEFT_ARM
//#define LIMB LIMB_RIGHT_LEG
//#define LIMB LIMB_LEFT_LEG

#if LIMB == LIMB_RIGHT_ARM /* Pick a single limb to be the BLE server */
#define IS_BLE_SERVER 1
#else
#define IS_BLE_SERVER 0
#endif


#if IS_BLE_SERVER
  #include "ArmsLengthServer.h"
  #define BLEDeviceType ArmsLengthServer
#else
  #include "ArmsLengthClient.h"
  #define BLEDeviceType ArmsLengthClient
#endif

// Note: Storing pointers in vectors is not best practice, but we have a simple
// architecture in which the IArduinoApplication instances are declared globally
// and are never destroyed during the lifetime of the program.
typedef std::vector<IArduinoApplication*> Applications;

#if LIMB == LIMB_HELMET

  #include "Helmet.h"
  #include "Limbs/Helmet.cpp"
  BLEDeviceType bleDevice("Helmet");
  Helmet limb(bleDevice);
  Applications applications = {&bleDevice, &limb};

#elif LIMB == LIMB_CHEST

  #include "Chest.h"
  #include "Limbs/Chest.cpp"
  BLEDeviceType bleDevice("Chest");
  Chest limb(bleDevice);
  Applications applications = {&bleDevice, &limb};

#elif LIMB == LIMB_BACK

  #include "Back.h"
  #include "Limbs/Back.cpp"
  BLEDeviceType bleDevice("Back");
  Back limb(bleDevice);
  Applications applications = {&bleDevice, &limb};

#elif LIMB == LIMB_RIGHT_ARM

  #include "RightArm.h"
  #include "Limbs/RightArm.cpp"
  #include "RightArmInputController.h"
  #include "Limbs/RightArmInputController.cpp"
  #include "ArmsLengthScripts.h"
  
  // For installation instructions see: https://github.com/mathertel/OneButton
  #include "lib/OneButton/src/OneButton.cpp"

  BLEDeviceType bleDevice("RightArm");
  RightArm limb(bleDevice);
  RightArmInputController controller(bleDevice);
  ArmsLengthScripts scripts(bleDevice);
  Applications applications = {&bleDevice, &limb, &controller, &scripts};

#elif LIMB == LIMB_LEFT_ARM

  #include "LeftArm.h"
  #include "Limbs/LeftArm.cpp"
  #include "LeftArmInputController.h"
  #include "Limbs/LeftArmInputController.cpp"
  
  // For installation instructions see: https://github.com/mathertel/OneButton
  #include "lib/OneButton/src/OneButton.cpp"
  
  BLEDeviceType bleDevice("LeftArm");
  LeftArm limb(bleDevice);
  LeftArmInputController controller(bleDevice);
  Applications applications = {&bleDevice, &limb, &controller};

#elif LIMB == LIMB_RIGHT_LEG

  #include "RightLeg.h"
  #include "Limbs/RightLeg.cpp"

  BLEDeviceType bleDevice("RightLeg");
  RightLeg limb(bleDevice);
  Applications applications = {&bleDevice, &limb};

#elif LIMB == LIMB_LEFT_LEG

  #include "LeftLeg.h"
  #include "Limbs/LeftLeg.cpp"

  BLEDeviceType bleDevice("LeftLeg");
  LeftLeg limb(bleDevice);
  Applications applications = {&bleDevice, &limb};

#else
  #error "Please define a valid LIMB value"
#endif

// the setup function runs once when you press reset or power the board
void setup() {
  //Serial1.end(); 

  Serial.begin(9600); 
  Serial.println( bleDevice.getDeviceName() );

  for (IArduinoApplication* application : applications)
  {
    application->setup();
  }
}

// the loop function runs over and over again forever
void loop() {

  for (IArduinoApplication* application : applications)
  {
    application->loop();
  }
}
