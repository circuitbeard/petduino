/*
 *    PushIt.ino - PushIt game example using the Petduino library
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

#define INTRO_ANIM_FRAMES 2
byte introF[INTRO_ANIM_FRAMES][8]={
  {
    B00100100,
    B00100100,
    B00100100,
    B00011000,
    B00000000,
    B00111100,
    B00111100,
    B11111111
  },
  {
    B00100100,
    B00100100,
    B00100100,
    B00100100,
    B00100100,
    B00011000,
    B00111100,
    B11111111
  }
};
unsigned long introD[INTRO_ANIM_FRAMES] = { 500, 500 };

#define NUMBER_COUNT 10
byte numbers[NUMBER_COUNT][8] = {
  { 0xE0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xE0 },
  { 0x60, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 },
  { 0xE0, 0x20, 0x20, 0xE0, 0x80, 0x80, 0x80, 0xE0 },
  { 0xE0, 0x20, 0x20, 0xE0, 0x20, 0x20, 0x20, 0xE0 },
  { 0xA0, 0xA0, 0xA0, 0xE0, 0x20, 0x20, 0x20, 0x20 },
  { 0xE0, 0x80, 0x80, 0xE0, 0x20, 0x20, 0x20, 0xE0 },
  { 0xE0, 0x80, 0x80, 0xE0, 0xA0, 0xA0, 0xA0, 0xE0 },
  { 0xE0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 },
  { 0xE0, 0xA0, 0xA0, 0xE0, 0xA0, 0xA0, 0xA0, 0xE0 },
  { 0xE0, 0xA0, 0xA0, 0xE0, 0x20, 0x20, 0x20, 0xE0 }
};

unsigned int count = 0;
unsigned int score = 0;
bool gameReady = true;
bool gameStarted = false;

unsigned int scoreOnes;
unsigned int scoreTens;

#define INTRO_STATE 0
#define GAME_START_STATE 1
#define GAME_LOOP_STATE 2
#define GAME_OVER_STATE 3

Petduino pet = Petduino();

void setup() {
  
  Serial.begin(9600);

  // Setup Petduino
  pet.begin();

  // Set initial state
  pet.setState(INTRO_STATE);

}

void loop() {

  // Update pet
  pet.update();
  
  // Check buttons
  if(pet.isBtn1Pressed()){
    if(gameReady){
      pet.setState(GAME_START_STATE); 
    }  else if(gameStarted) {
      score++;
    }
  }
  
  if(pet.isBtn2Pressed()){
    if(gameReady){
      pet.setState(GAME_START_STATE); 
    } else if(gameStarted) {
      score++;
    }
  }

  // Update based on state
  switch(pet.getState()) {
    
    case INTRO_STATE:
    
      // Stop any pevious games
      gameReady = true;
    
      // Show intro anim
      pet.playAnimation(introF, introD, INTRO_ANIM_FRAMES, 1);
      pet.setNextState(INTRO_STATE, 1000); // Wait for button press
      
      break;
      
    case GAME_START_STATE:
    
      // Flag game started
      gameReady = false;
      gameStarted = true;
      
      // Stop the intro animation      
      pet.stopAnimation();
      
      // Reset variables
      score = 0;
      count = 0;
    
      pet.setState(GAME_LOOP_STATE);
      break;
      
    case GAME_LOOP_STATE:
    
      if(count < 64) {
    
        count++;
        
        // Draw bubbles
        for(int r=0;r<8;r++){
          byte row = 0x00;
          for(int c=0;c<8;c++){
            if((r*8)+c > count){
              row |= (0x80 >> c);
            }  
          } 
          pet.drawRow(r, row);
        } 
      
        // Loop
        pet.setNextState(GAME_LOOP_STATE, 60);
        
      } else {
        
        gameStarted = false;
        pet.setState(GAME_OVER_STATE);
        
      }
      break;
      
    case GAME_OVER_STATE:
    
      // Split the score digits
      scoreTens = score/10;
      scoreOnes = score-scoreTens*10;
    
      // Generate & draw score number graphic
      for(int b = 0; b < 8; b++){
        pet.drawRow(b, numbers[scoreTens][b] | numbers[scoreOnes][b] >> 4);
      }
      
      // Go back to intro
      pet.setNextState(INTRO_STATE, 5000);
      break;
      
  }

}
