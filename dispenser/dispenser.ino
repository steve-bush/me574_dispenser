#include "dispenser.h"

enum DispenserState { Unwashed, Dispense, Washed } dispenserState;
unsigned long SensorTime; // Timer for when sensor detects hand.
unsigned long WashTime; // Timer for end of washed state
unsigned long DispenseTime; // Timer for length of dispense state
bool wash_flag; // Flag for deciding what to send
unsigned int previous_readings[NUM_READINGS];
unsigned int new_readings[NUM_READINGS];
int index = 0;
// 1 minute 30 seconds to log on (milliseconds)
const unsigned long WASH_TIMEOUT = 90000;

void set_to_washed() {
  wash_flag = WASHED; // Indicate that button is pressed
  analogWrite(RED_LED, 0); // Turn off red first so both LEDs are not on
  analogWrite(GREEN_LED, 255); // Turn on green to indicate login time
  WashTime = millis(); // Begine wash_timer for timeout
}

void set_to_unwashed() {
  wash_flag = UNWASHED; // Indicate unwashed
  analogWrite(GREEN_LED, 0); // Turn off green first so both LEDs are not on
  analogWrite(RED_LED, 255); // Turn on red to indicate washing hands
}

void initialize_sensor() {
  // Set up the sensor reading arrayss
  for (int i = 0; i < NUM_READINGS; i++) {
    previous_readings[i] = analogRead(PHOTODIODE_SENSOR);
    new_readings[i] = analogRead(PHOTODIODE_SENSOR);
  }
}

bool read_sensor() {
  if (millis() - SensorTime < SENSOR_DELAY){
    return false;
  }
  //Add sensor reading into the array
  previous_readings[index] = new_readings[index];
  new_readings[index] = analogRead(PHOTODIODE_SENSOR);

  // Move index down the array
  index++;

  // Loop index back to 0 if past array bounds
  if (index > NUM_READINGS - 1) {
    index = 0;
  }

  // Initialize average readings variables
  float previous_ave = 0;
  float new_ave = 0;
  // Add up the previous and new readings arrays
  for (int i = 0; i < NUM_READINGS; i++) {
    previous_ave += previous_readings[i];
    new_ave += new_readings[i];
  }

  // Calculate the average of the readings
  previous_ave /= NUM_READINGS;
  new_ave /= NUM_READINGS;

  // Update SensorTime for delay
  SensorTime = millis();
  
  if (previous_ave - new_ave > THRESHOLD_CHANGE) {
    return true;
  }
  else {
    return false;
  }
}

void setup() {
  // Setup pins
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(MOTOR, OUTPUT);
  Serial.begin(9600); // Set baud rate to 9600.
  pinMode(PHOTODIODE_SENSOR, INPUT);
  pinMode(PHOTODIODE_POWER, OUTPUT);
  digitalWrite(PHOTODIODE_POWER, HIGH);
  pinMode(IR_LED, OUTPUT);
  digitalWrite(IR_LED, HIGH);
  set_to_unwashed();
  initialize_sensor();
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
      if (read_sensor()) { // If hand detected.
        DispenseTime = millis(); // Record time of hand detection.
        analogWrite(MOTOR, 255);
        set_to_washed();
        dispenserState = Dispense; // Set state back to unwashed
      }
      break;

    case Dispense:
      read_sensor();
      if (millis() - DispenseTime >= DISPENCE_TIME) {
        analogWrite(MOTOR, 0);
        set_to_washed();
        dispenserState = Washed; // Set state to washed
      }
      break;

    case Washed:
      if (read_sensor() && millis() - DispenseTime >= DISPENCE_DELAY) { // If hand detected.
        analogWrite(MOTOR, 255);
        DispenseTime = millis(); // Record time of hand detection.
        dispenserState = Dispense; // Move to intial detection state.
      }
      else if (millis() - WashTime >= WASH_TIMEOUT) {
        set_to_unwashed();
        dispenserState = Unwashed; // Set state back to unwashed
      }
      break;
  }
} // loop()
