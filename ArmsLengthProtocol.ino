// =============================================================================
// Arm's Length Protocol
//
// Description:
//   An Iron Man suit controller built around Arduino, Bluetooth Low Energy,
//   and the Xiao ESP32 C6 microncontroller.
//   This project is inspired by https://github.com/crashworks3d/Iron_Man_Servo,
//   but uses none of its code.
//
// Dependencies:
//   Arduino Libraries:
//     OneButton by Matthias Hertel 2.6.1
//     Adafruit Neopixel 1.15.2
//     FastLED by Daniel Garcia 3.10.3
//     ESP32PWM
//     ESP32Servo
// =============================================================================

#include <Arduino.h>
#include "ArmsLengthProtocol.h"
#include "NullController.h"

#define LIMB_HELMET 1
#define LIMB_CHEST 2
#define LIMB_BACK 3
#define LIMB_RIGHT_ARM 4
#define LIMB_LEFT_ARM 5
#define LIMB_RIGHT_LEG 6
#define LIMB_LEFT_LEG 7

#define LIMB LIMB_RIGHT_ARM

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
  BLEDeviceType bleDevice("Helmet");
  Helmet limb(bleDevice);
  NullController controller(bleDevice);
  Applications applications = {&bleDevice, &limb, &controller};
#elif LIMB == LIMB_CHEST
  #include "Chest.h"
  BLEDeviceType bleDevice("Chest");
  Chest limb(bleDevice);
  NullController controller(bleDevice);
  Applications applications = {&bleDevice, &limb, &controller};
#elif LIMB == LIMB_BACK
  #include "Back.h"
  BLEDeviceType bleDevice("Back");
  Back limb(bleDevice);
  NullController controller(bleDevice);
  Applications applications = {&bleDevice, &limb, &controller};
#elif LIMB == LIMB_RIGHT_ARM
  #include "RightArm.h"
  #include "RightArmInputController.h"
  #include "ArmsLengthScripts.h"
  BLEDeviceType bleDevice("RightArm");
  RightArm limb(bleDevice);
  RightArmInputController controller(bleDevice);
  ArmsLengthScripts scripts(bleDevice);
  Applications applications = {&bleDevice, &limb, &controller, &scripts};
#elif LIMB == LIMB_LEFT_ARM
  #include "LeftArm.h"
  BLEDeviceType bleDevice("LeftArm");
  LeftArm limb(bleDevice);
  NullController controller(bleDevice);
  Applications applications = {&bleDevice, &limb, &controller};
#elif LIMB == LIMB_RIGHT_LEG
  #include "RightLeg.h"
  BLEDeviceType bleDevice("RightLeg");
  RightLeg limb(bleDevice);
  NullController controller(bleDevice);
  Applications applications = {&bleDevice, &limb, &controller};
#elif LIMB == LIMB_LEFT_LEG
#include "LeftLeg.h"
  BLEDeviceType bleDevice("LeftLeg");
  LeftLeg limb(bleDevice);
  NullController controller(bleDevice);
  Applications applications = {&bleDevice, &limb, &controller};
#else
  #error "Please define a valid LIMB value"
#endif



//#include "ESP32Servo.h"


//const int MY_PIN = D9;

//Servo myservo;  // create servo object to control a servo
// 16 servo objects can be created on the ESP32

//int pos = 0;    // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
// Possible PWM GPIO pins on the ESP32-S2: 0(used by on-board button),1-17,18(used by on-board LED),19-21,26,33-42
// Possible PWM GPIO pins on the ESP32-S3: 0(used by on-board button),1-21,35-45,47,48(used by on-board LED)
// Possible PWM GPIO pins on the ESP32-C3: 0(used by on-board button),1-7,8(used by on-board LED),9-10,18-21

//int servoPin = D10;
// D0 yes
// D1 yes
// D2 yes
// D3 yes
// D4 yes
// D5 yes
// D6 yes
// D7 yes
// D8 yes
// D9 yes
// D10 yes


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
 // pinMode(MY_PIN, OUTPUT);


	// Allow allocation of all timers
//	ESP32PWM::allocateTimer(0);
//	ESP32PWM::allocateTimer(1);
//	ESP32PWM::allocateTimer(2);
//	ESP32PWM::allocateTimer(3);
//	myservo.setPeriodHertz(50);    // standard 50 hz servo
//	myservo.attach(servoPin, 1000, 2000); // attaches the servo on pin 18 to the servo object
	// using default min/max of 1000us and 2000us
	// different servos may require different min/max settings
	// for an accurate 0 to 180 sweep
  
  Serial1.end(); 

  Serial.begin(9600); 
  Serial.println( F("Hello World") );

  for (IArduinoApplication* application : applications)
  {
    application->setup();
  }
}

// the loop function runs over and over again forever
void loop() {
 // digitalWrite(MY_PIN, HIGH);  // turn the LED on (HIGH is the voltage level)
 // delay(1000);                      // wait for a second


	// for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
	// 	// in steps of 1 degree
	// 	myservo.write(pos);    // tell servo to go to position in variable 'pos'
	// 	delay(15);             // waits 15ms for the servo to reach the position
	// }

  //digitalWrite(MY_PIN, LOW);   // turn the LED off by making the voltage LOW
  //delay(1000);                      // wait for a second

	// for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
	// 	myservo.write(pos);    // tell servo to go to position in variable 'pos'
	// 	delay(15);             // waits 15ms for the servo to reach the position
	// }


  for (IArduinoApplication* application : applications)
  {
    application->loop();
  }
}
