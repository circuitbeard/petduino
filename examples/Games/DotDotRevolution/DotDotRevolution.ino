/*
 *    DotDotRevolution.ino - Dot dot revolution game example using the Petduino library
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

byte intro[8]= {
  B01100000,
  B11110000,
  B11110000,
  B01100000,
  B00000110,
  B00001111,
  B00001111,
  B00000110
};

#define COUNTDOWN_FRAME_COUNT 5
byte countdownF[COUNTDOWN_FRAME_COUNT][8]={
  {
    B00111100,
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100
  },
  {
    B00110000,
    B01110000,
    B11110000,
    B11110000,
    B11111111,
    B11111111,
    B01111110,
    B00111100
  },
  {
    B00110000,
    B01110000,
    B11110000,
    B11110000,
    B11110000,
    B11110000,
    B01110000,
    B00110000
  },
  {
    B00110000,
    B01110000,
    B11110000,
    B11110000,
    B00000000,
    B00000000,
    B00000000,
    B00000000
  },
  {
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000
  }
};
unsigned long countdownD[COUNTDOWN_FRAME_COUNT] = { 1000, 1000, 1000, 1000, 1000 };

#define NUMBER_COUNT 10
byte numbers[NUMBER_COUNT][8] = {
  {
    B11100000,
    B10100000,
    B10100000,
    B10100000,
    B10100000,
    B10100000,
    B10100000,
    B11100000,
  },
  {
    B01100000,
    B00100000,
    B00100000,
    B00100000,
    B00100000,
    B00100000,
    B00100000,
    B00100000,
  },
  {
    B11100000,
    B00100000,
    B00100000,
    B11100000,
    B10000000,
    B10000000,
    B10000000,
    B11100000,
  },
  {
    B11100000,
    B00100000,
    B00100000,
    B11100000,
    B00100000,
    B00100000,
    B00100000,
    B11100000,
  },
  {
    B10100000,
    B10100000,
    B10100000,
    B11100000,
    B00100000,
    B00100000,
    B00100000,
    B00100000,
  },
  {
    B11100000,
    B10000000,
    B10000000,
    B11100000,
    B00100000,
    B00100000,
    B00100000,
    B11100000,
  },
  {
    B11100000,
    B10000000,
    B10000000,
    B11100000,
    B10100000,
    B10100000,
    B10100000,
    B11100000,
  },
  {
    B11100000,
    B00100000,
    B00100000,
    B00100000,
    B00100000,
    B00100000,
    B00100000,
    B00100000,
  },
  {
    B11100000,
    B10100000,
    B10100000,
    B11100000,
    B10100000,
    B10100000,
    B10100000,
    B11100000,
  },
  {
    B11100000,
    B10100000,
    B10100000,
    B11100000,
    B00100000,
    B00100000,
    B00100000,
    B11100000,
  }
};

byte gameBoard[8] {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00011000
};

unsigned int score = 0;
unsigned int lives = 6;

byte emptyMask = B00000000;
byte leftMask = B11000000;
byte rightMask = B00000011;
byte leftAndRightMask = B11000011;
byte middleMask = B00011000;

byte prevRowMask;
byte lastRowMask;
long nextMove;
bool gameStarted = false;
bool btn1Pressed = false;
bool btn2Pressed = false;
unsigned long speed;

unsigned int scoreOnes;
unsigned int scoreTens;

#define INTRO_STATE 0
#define COUNTDOWN_STATE 1
#define GAME_START_STATE 2
#define GAME_LOOP_STATE 3
#define GAME_OVER_STATE 4

Petduino pet = Petduino();

void setup() {

  // Setup Petduino
  pet.begin();

  // Set initial state
  pet.setState(INTRO_STATE);

  // Initialize random seed
  randomSeed(analogRead(0));

}

void loop() {

  // Update pet
  pet.update();
  
  // Check buttons
  if(pet.isBtn1Pressed()){
    if(!gameStarted){
      pet.setState(COUNTDOWN_STATE); 
    } else {
      btn1Pressed = true;
    }
  }
  
  if(pet.isBtn2Pressed()){
    if(!gameStarted){
      pet.setState(COUNTDOWN_STATE); 
    } else {
      btn2Pressed = true;
    }
  }
  
  // Update based on state
  switch(pet.getState()) {
    
    case INTRO_STATE:
      pet.drawImage(intro);
      pet.wait(); // Wait for button press
      break;
      
    case COUNTDOWN_STATE:
      pet.playAnimation(countdownF, countdownD, COUNTDOWN_FRAME_COUNT, 1);
      pet.setNextState(GAME_START_STATE, 5000);
      break;
      
    case GAME_START_STATE:
    
      // Reset variables
      score = 0;
      lives = 6;
      gameStarted = true;
      
      // Reset board
      for(int i=0; i<8; i++){
        gameBoard[i] == gameBoard[i] & middleMask;
      }
      
      // Start game
      pet.setState(GAME_LOOP_STATE);
      break;
      
    case GAME_LOOP_STATE:
    
      // Check buttons
      lastRowMask = gameBoard[7] & leftAndRightMask;
      
      if(lastRowMask == leftAndRightMask) {
         if(btn1Pressed && btn2Pressed){
           score++;
         } else {
           lives--; 
         }
      } else if(lastRowMask == leftMask) {
         if(btn1Pressed && !btn2Pressed){
           score++;
         } else {
           lives--; 
         }
      } else if(lastRowMask == rightMask) {
         if(!btn1Pressed && btn2Pressed){
           score++;
         } else {
           lives--; 
         }
      } else if(lastRowMask == emptyMask) {
          if(btn1Pressed || btn2Pressed) {
            lives--;
          }
      }
      
      // Shift items down
      for(int i = 7; i>=1; --i){
        prevRowMask = gameBoard[i-1] & leftAndRightMask;
        gameBoard[i] = gameBoard[i] & middleMask;
        gameBoard[i] = gameBoard[i] | prevRowMask;
      }
      
      // Choose next move
      gameBoard[0] = gameBoard[0] & middleMask;

      nextMove = random(0, 6); 
      if(nextMove == 1) {
        gameBoard[0] = gameBoard[0] | leftMask;
      } else if(nextMove == 2) {
        gameBoard[0] = gameBoard[0] | rightMask;
      } else if(nextMove == 3) {
        gameBoard[0] = gameBoard[0] | leftAndRightMask;
      }
      
      // Update lives
      for(int i=5; i>=0; --i){
        if(i+1 > lives){
          gameBoard[5-i] = gameBoard[5-i] & leftAndRightMask;
        } else {
          gameBoard[5-i] = gameBoard[5-i] | middleMask;
        }
      }
    
      // Reset button presses
      btn1Pressed = false;
      btn2Pressed = false;
      
      // Increase speed as score goes up
      if(score > 70) {
        speed = 300;
      } else if(score > 60) {
        speed = 350;
      } else if(score > 50) {
        speed = 400;
      } else if(score > 40) {
        speed = 450;
      } else if(score > 30) {
        speed = 500;
      } else if(score > 20) {
        speed = 550;
      } else if(score > 10) {
        speed = 600;
      } else {
        speed = 700; 
      }
      
      // Check for game over
      if(lives <= 0) {
        
        pet.setState(GAME_OVER_STATE);
        
      } else {
        
        // Draw the board and loop
        pet.drawImage(gameBoard);
        pet.setNextState(GAME_LOOP_STATE, speed);
        
      }
      
      break;
      
    case GAME_OVER_STATE:
    
      // Stop the game
      gameStarted = false;
      
      // Split the score digits
      scoreTens = score/10;
      scoreOnes = score-scoreTens*10;
    
      // Generate & draw score number graphic
      for(int b = 0; b < 8; b++){
        pet.drawRow(b, numbers[scoreTens][b] | numbers[scoreOnes][b] >> 4);
      }
      
      // Go back to intro
      pet.setNextState(INTRO_STATE, 3000);
      break;
      
  }

}
