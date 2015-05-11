/*
 *    Theremin.ino - Theremin example using the Petduino library
 *    Copyright (c) 2015 Circuitbeard
 *
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 *
 *    This permission notice shall be included in all copies or
 *    substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */

#include <LedControl.h>
#include <Petduino.h>

int speed = 20;

byte screen[8];

#define LOOP_STATE 0

Petduino pet = Petduino();

void setup() {

  // Setup Petduino
  pet.begin();

  // Set initial state
  pet.setState(LOOP_STATE);

}

void loop() {

  // Update pet
  pet.update();


  // Update based on state
  switch(pet.getState()) {
      
    case LOOP_STATE:
    
        // Shift all screens bits left
        for(int r=0;r<8;r++){
           screen[r] = screen[r] << 1; 
        }
        
        // Only play a sound of a button is held down
        if(pet.isBtn1Held() || pet.isBtn2Held()){ 
        
          // Sample light level  
          int lightLevel = pet.getLightLevel();
          
          // Play tone
          int toneToPlay = map(lightLevel, 0, 1023, 261, 524); 
          pet.playTone(toneToPlay, speed);
          
          // Append level to screen
          int level = map(lightLevel, 0, 1023, 0, 8);
          for(int r=(8-level);r<8;r++) {
             screen[r] |= 1;
          }
          
        }
        
        // Draw screen
        pet.drawImage(screen);
      
        // Loop
        pet.setNextState(LOOP_STATE, speed);

      break;
      
  }

}

int wrap(int num, int numMax) {
  if(num < 0){
    while(num < 0){
      num += numMax; 
    }
    return num;
  } else {
    return num % numMax; 
  }
}
