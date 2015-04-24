#include <LedControl.h>
#include <Petduino.h>

byte alien1[8]={
  B00011000,
  B00111100,
  B01111110,
  B11011011,
  B11111111,
  B01011010,
  B10000001,
  B01000010
};

byte alien2[8]={
  B00011000,
  B00111100,
  B01111110,
  B11011011,
  B11111111,
  B00100100,
  B01011010,
  B10100101
};

Petduino pet = Petduino();

void setup() {
  
  // Setup Petduino
  pet.begin();
  
  // Set initial state
  pet.setState(1);
  
}

void loop() {
  
  // Call pet loop
  pet.loop();
  
  // Update display based on current state
  switch(pet.getState()){
    
    case 1: // Alien1
      pet.drawImage(alien1);
      pet.playTone(200, 200);
      pet.setNextState(2, 1000);
      break;
      
    case 2: // Alien2
      pet.drawImage(alien2);
      pet.playTone(100, 200);
      pet.setNextState(1, 1000);
  }

}
