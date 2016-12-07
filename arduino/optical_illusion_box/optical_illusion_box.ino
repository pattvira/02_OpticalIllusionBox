/*

    02_Optical_Illusion_Box
    http://pattvira.com/optical_illusion_box

*/

#include <AccelStepper.h>              // AccelStepper Library can be downloaded here: http://www.airspayce.com/mikem/arduino/AccelStepper/index.html
// To import a library: Go to Sketch > Import Library > Add Library > [your downloaded file]

AccelStepper stepper(4, 2, 3, 4, 5);   // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

int ledPin = 11;      // LED connected to pin 11
int switchPin = 8;    // ON/OFF switch connected to pin 8
int potPin = A0;      // Potentiometer connected to pin A0

// Variables for ON/OFF switch
int switchState = 0;
int lastSwitchState = 0;
boolean light = false;

// Variables for offseting strobe light
unsigned long interval = 1000;              // Variable for an offset interval
boolean ledState = HIGH;                    // Variable for LED state
unsigned long previousMillis = 0;

float strobe = 100;                         // Variable for strobe light on
int velocity = 200;                         // Variable for stepper motor speed (number if steps per second)
                                            // My motor rotates 1.8 degrees each turn -> 200 steps per revolution
int maximumSpeed = 512;
int steadySpeed = 400;

void setup() {

  stepper.setMaxSpeed(maximumSpeed);        // Define the maximum motor speed
  stepper.setSpeed(steadySpeed);	    // Initialize the motor speed
  pinMode(ledPin, OUTPUT);                  // Set an LED pin as an output pin
  pinMode(switchPin, INPUT);                // Set a on/off switch pin as an input pin

}


void loop() {

  switchState = digitalRead(switchPin);               // Identify if the switch is on or off
  strobe = map(analogRead(potPin), 0, 1023, 0, 100);  // Strobe frequency
  if (switchState != lastSwitchState) {
    if (switchState == HIGH) {
      light = true;
    } else {
      light = false;
    }
  }
  lastSwitchState = switchState;

  if (light) {                                        // If switch is on,
    if (strobe == 0) {
      strobe = 1;
    } else {
      strobeLight(ledPin, 1, strobe - 1);             // Light strobes
    }
    stepper.setSpeed(steadySpeed);                    // Motor is set to a constant speed
                                
  } else {                                            // If switch is off,
    digitalWrite(ledPin, LOW);                        // Light turns off
    stepper.setSpeed(0);                              // Motor stops
  }

  stepper.runSpeed();                                 // Motor only runs when this command is called

}


// Function for turning on and off the LED strip without using delay 
void strobeLight(int pin, int lightOn, int lightOff) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;                      // Save the last time you blinked the LED

    if (ledState == LOW) {                               // If the LED is off turn it on and vice-versa
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState);                      // Set the LED with the ledState of the variable

    if (ledState)                                        // Set the offset interval
    {
      interval = lightOn;
    }
    else
    {
      interval = lightOff;
    }
  }
}
