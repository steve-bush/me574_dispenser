//Variables for which pin to debounce, button states, amd timer.
enum ButtonState { ButtonIdle, ButtonWait, ButtonLow } buttonState;
unsigned long ButtonTime;

//Function to set up system to read button press.
int ButtonInitialize()
{
  pinMode( 4, INPUT ); //Sets pin to INPUT.
  pinMode( 13, OUTPUT ); //Debug
  digitalWrite( 13, LOW ); //Debug
  buttonState = ButtonIdle; // Sets state to idle.
} //End ButtonInitialize

//Function that can be called to service the switch.
int ButtonNextState(  )
{
  switch ( buttonState ) {
    case ButtonIdle:
      if ( digitalRead( 4 ) == LOW ) { // If button pressed.
        ButtonTime = millis(); // Record time of high to low transition.
        buttonState = ButtonWait; // Move to wait state.
        digitalWrite( 13, HIGH ); // Turn on LED. Debug
      }
      break;
    case ButtonWait:
      if ( digitalRead( 4 ) == HIGH ) { // If button is not pressed
        buttonState = ButtonIdle; // Set state back to idle
      }
      else if ( millis() - ButtonTime > 5 ) { // If button pressed for 5 milliseconds
        buttonState = ButtonLow; // Move state to LOW.
        digitalWrite( 13, LOW ); // Turn off LED. Debug
        return 1; // Indicate that button has been pressed.
      }
      break;
    case ButtonLow:
      if ( digitalRead( 4 ) == HIGH ) { // Once button released
        buttonState = ButtonIdle; // Go back to idle state.
        return 2; // Indicate that button has been released.
      }
      break;
  } //End switch
  return 0; // By default return 0 so nothing happens.
}//End loop.
