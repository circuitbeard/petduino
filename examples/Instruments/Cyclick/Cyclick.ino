/*
 *    Cyclick.ino - Cyclick synth example using the Petduino library
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

#define WAIT_ANIM_FRAMES 2
byte waitF[WAIT_ANIM_FRAMES][8]={
  { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 },
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};
unsigned long waitD[WAIT_ANIM_FRAMES] = { 300, 300 };

bool waiting = true;

int cursorX = 0;
int cursorY = 0;
bool cursorFlash = false;

int playHeadX = 0;
int speed = 300;

bool btn1Pressed = false;
bool btn2Pressed = false;

int tones[8] = { 261, 294, 329, 349, 392, 440, 493, 523 };
byte sequence[8];
byte screen[8];

#define WAIT_STATE 0
#define LOOP_STATE 1

Petduino pet = Petduino();

void setup() {

  // Setup Petduino
  pet.begin();

  // Set initial state
  pet.setState(WAIT_STATE);

}

void loop() {

  // Update pet
  pet.update();
  
  // Check buttons
  if(pet.isBtn1Pressed()){
    if(waiting) {
      waiting = false;
      pet.stopAnimation();
      pet.setState(LOOP_STATE);
    } else {
      btn1Pressed = true;
    }
  }
  
  if(pet.isBtn2Pressed()){
    if(waiting) {
      waiting = false;
      pet.stopAnimation();
      pet.setState(LOOP_STATE);
    } else {
      btn2Pressed = true;
    }
  }

  // Update based on state
  switch(pet.getState()) {
    
    case WAIT_STATE:
    
      pet.playAnimation(waitF, waitD, WAIT_ANIM_FRAMES, 1);
      pet.setNextState(WAIT_STATE, 600);
      break;
      
    case LOOP_STATE:
    
        // Clear the screen
        clearScreen();
        
        // Handle key presses
        if(btn1Pressed && btn2Pressed) {
          bool isOn = sequence[cursorY] & (0x80 >> cursorX);
          for(int r=0;r<8;r++){ // Turn off all in column
            sequence[r] &= ~(0x80 >> cursorX);
          }
          if(!isOn){ // Turn on selected cell (if was previously off)
            sequence[cursorY] |= (0x80 >> cursorX);
          }
        } else if (btn1Pressed) {
          cursorY = wrap(cursorY + 1, 8);
        } else if (btn2Pressed) {
           cursorX = wrap(cursorX + 1, 8);
        }
        
        // Move playhead
        playHeadX = wrap(playHeadX + 1, 8);
        
        // Draw sequence
        for(int r=0; r<8;r++) {
          for(int c=0;c<8;c++) {
            if((r != cursorY || c != cursorX) && (sequence[r] & (0x80 >> c))){
               screen[r] |= (0x80 >> c);
            } 
          }
        }
        
        // Draw playhead & play notes
        for(int r=0;r<8;r++){
          screen[r] ^= (0x80 >> playHeadX); 
          if(sequence[r] & (0x80 >> playHeadX)) {
            pet.playTone(tones[r], speed); 
          }
        }
        
        // Draw cursor
        if(cursorFlash) {
          screen[cursorY] |= (0x80 >> cursorX); 
        }
        
        // Draw screen
        pet.drawImage(screen);
        
        // Flash cursor
        cursorFlash = !cursorFlash;
        
        // Reset btn presses
        btn1Pressed = false;
        btn2Pressed = false;
      
        // Loop
        pet.setNextState(LOOP_STATE, speed);

      break;
      
  }

}

void clearScreen() {
  for(unsigned int r=0;r<8;r++){
    screen[r] = 0x00;
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
