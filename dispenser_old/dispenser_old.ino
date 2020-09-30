#include "dispenser.h"

enum DispenserState { Unwashed, HandDetected, Dispense, Washed } dispenserState;
unsigned long SensorTime; // Timer for when sensor detects hand.
unsigned long WashTime; // Timer for end of washed state
unsigned long DispenseTime; // Timer for length of dispense state
bool wash_flag; // Flag for deciding what to send
// 1 minute 30 seconds to log on (milliseconds)
const unsigned long WASH_TIMEOUT = 90000;

void set_to_washed() {
  wash_flag = WASHED; // Indicate that button is pressed
  analogWrite(RED_LED, 0); // Turn off red first so both LEDs are not on
  analogWrite(GREEN_LED, 255); // Turn on green to indicate login time
  WashTime = millis(); // Begine wash_timer for timeout
} // End set_to_wash.

void set_to_unwashed() {
  wash_flag = UNWASHED; // Indicate unwashed
  analogWrite(GREEN_LED, 0); // Turn off green first so both LEDs are not on
  analogWrite(RED_LED, 255); // Turn on red to indicate washing hands
}

void setup() {
  // Setup pins
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(MOTOR, OUTPUT);
  analogWrite(MOTOR, 0);
  Serial.begin(9600); // Set baud rate to 9600.
  pinMode(PHOTODIODE_SENSOR, INPUT);
  pinMode(PHOTODIODE_POWER, OUTPUT);
  digitalWrite(PHOTODIODE_POWER, HIGH);
  pinMode(IR_LED, OUTPUT);
  digitalWrite(IR_LED, HIGH);
  set_to_unwashed();
  dispenserState = Unwashed; // Set state back to unwashed
} // setup()

void loop() {
    // Check for ping from pc
  if (Serial.available() > 0) {
    Serial.readString(); // Read in passed
    // Send state of wash_flag to pc
    if (wash_flag) {
      Serial.println("Washed"); // Indicate that hands have been washed
      set_to_unwashed();
      dispenserState = Unwashed; // Set state back to unwashed
    }
    else {
      Serial.println("Unwashed"); // Indicate that hands are not washed
    }
  }

  switch (dispenserState) {
    case Unwashed:
      if (analogRead(PHOTODIODE_SENSOR) <= THRESHOLD) { // If hand detected.
        SensorTime = millis(); // Record time of hand detection.
        dispenserState = HandDetected; // Move to intial detection state.
      }
      break;

    case HandDetected:
      if (analogRead(PHOTODIODE_SENSOR) > THRESHOLD) { // If hand is not detected
        set_to_unwashed();
        dispenserState = Unwashed; // Set state back to unwashed
      }
      else if (millis() - SensorTime > 10) { // If sensor detects hands for 5 milliseconds
        analogWrite(MOTOR, 255);
        DispenseTime = millis(); // Start timer for length of dispense state.
        dispenserState = Dispense; // Move state to sensed.
      }
      break;

    case Dispense:
      if (millis() - DispenseTime >= DISPENCE_TIME) {
        analogWrite(MOTOR, 0);
        set_to_washed();
        dispenserState = Washed; // Set state to washed
      }
      break;

    case Washed:
      if (analogRead(PHOTODIODE_SENSOR) <= THRESHOLD && millis() - DispenseTime >= DISPENCE_DELAY) { // If hand detected.
        SensorTime = millis(); // Record time of hand detection.
        dispenserState = HandDetected; // Move to intial detection state.
      }
      else if (millis() - WashTime >= WASH_TIMEOUT or !wash_flag) {
        set_to_unwashed();
        dispenserState = Unwashed; // Set state back to unwashed
      }
      break;
  }
} // loop()
