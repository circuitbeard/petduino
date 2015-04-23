#include "Petduino.h"

Petduino *thisPetduino = NULL;

Petduino::Petduino() {
}

// Setup inputs, outputs, timers, etc. Call this from main arduino setup() method!!
void Gamer::setup(){
  ::thisPetduino = this;
  
  // Setup outputs
  
  // Setup inputs
  
}

// Private -------------------------------------------

// Public --------------------------------------------

// Set the value of the Petduino LED
void Petduino::setLED(bool value) {
  digitalWrite(LED, value);
}

// Toggle the Petduino LED
void Petduino::toggleLED() {
  digitalWrite(LED, !digitalRead(LED));
}
