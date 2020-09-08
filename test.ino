#include "ButtonDebounce.h"

#define WASHED true // Characters to be used for the wash_flag
#define UNWASHED false
#define GREEN A1 // LED pins
#define RED A0
#define WASHTIMEOUT 30*1000 // 30 seconds to log on
#define SPEAKER 2

bool wash_flag; // Flag for deciding what to send
unsigned long wash_timer; // Timer for end of washed state

void set_to_washed() {
  wash_flag = WASHED; // Indicate that button is pressed
  analogWrite(RED, 0); // Turn off red first so both LEDs are not on
  analogWrite(GREEN, 255); // Turn on green to indicate login time
  wash_timer = millis(); // Begine wash_timer for timeout
} // End set_to_wash.

void set_to_unwashed() {
  wash_flag = UNWASHED; // Indicate unwashed
  analogWrite(GREEN, 0); // Turn off green first so both LEDs are not on
  analogWrite(RED, 255); // Turn on red to indicate washing hands
}

void setup() { // Function run at start of program.
  ButtonInitialize(); // Calls function that sets up pin and machine.
  pinMode(GREEN, OUTPUT); // Set green led pin to output
  pinMode(RED, OUTPUT); //Set red led pin to output
  pinMode(SPEAKER, OUTPUT);
  Serial.begin(9600); // Set baud rate to 9600
  set_to_unwashed();
} //End setup.

//Function to check status of button.
void loop() {
  // Check for ping from pc
  if(Serial.available() > 0){
    String ping = Serial.readStringUntil('\n'); // Read in passed string until newline indicator
    // Send state of wash_flag to pc
    if(wash_flag) {
      Serial.println("Washed"); // Indicate that hands have been washed
      set_to_unwashed();
    }
    else {
      Serial.println("Unwashed"); // Indicate that hands are not washed
    }
  }
  // Change login state if user takes too long
  if(millis() - wash_timer > WASHTIMEOUT && wash_flag) {
    set_to_unwashed();
  }
  switch ( ButtonNextState( ) ) {
    case 1:
      digitalWrite(13, HIGH); // Indicate button has been pressed with LED.
      set_to_washed();
      tone(SPEAKER, 1000, 200);
      break;
    case 2:
      digitalWrite(13, LOW); // Indicate button released.
      break;
  } //End switch.
}//End loop
