/*
 *    FlappyBird.ino - Flappy bird game example using the Petduino library
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
    B00011000,
    B00100100,
    B00101100,
    B11100100,
    B10001110,
    B10010001,
    B01001110,
    B00111000
  },
  {
    B00011000,
    B00100100,
    B00101100,
    B00100100,
    B01001110,
    B10010001,
    B10101110,
    B11111000
  }
};
unsigned long introD[INTRO_ANIM_FRAMES] = { 500, 500 };

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

int birdY = 0;
int wingY = 0;

byte pipe1;
int pipe1X = 0;
unsigned int pipe1GapSize = 0;
unsigned int pipe1GapOffset = 0;

byte pipe2;
int pipe2X = 0;
unsigned int pipe2GapSize = 0;
unsigned int pipe2GapOffset = 0;

unsigned int score = 0;
unsigned int speed = 0;

bool oldBtnPressed = false;
bool btnPressed = false;
bool gameStarted = false;

byte gameBoard[8];

unsigned int scoreOnes;
unsigned int scoreTens;

#define INTRO_STATE 0
#define GAME_START_STATE 1
#define GAME_LOOP_STATE 2
#define GAME_OVER_STATE 3

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
      pet.setState(GAME_START_STATE); 
    } else {
      btnPressed = true;
    }
  }
  
  if(pet.isBtn2Pressed()){
    if(!gameStarted){
      pet.setState(GAME_START_STATE); 
    } else {
      btnPressed = true;
    }
  }

  // Update based on state
  switch(pet.getState()) {
    
    case INTRO_STATE:
    
      // Stop any pevious games
      gameStarted = false;
    
      // Show intro anim
      pet.playAnimation(introF, introD, INTRO_ANIM_FRAMES, 1);
      pet.setNextState(INTRO_STATE, 1000); // Wait for button press
      
      break;
      
    case GAME_START_STATE:
    
      // Flag game started
      gameStarted = true;
      
      // Stop the intro animation      
      pet.stopAnimation();
      
      // Reset variables
      score = 0;
      birdY = 0;
      wingY = 0;
      oldBtnPressed = false;
      btnPressed = false;
      
      // Generate pipes
      pipe1 = generatePipe();
      pipe1X = 7; // Screen size
      pipe2 = generatePipe();
      pipe2X = 11; // Screen size * 1.5
    
      pet.setState(GAME_LOOP_STATE);
      break;
      
    case GAME_LOOP_STATE:
    
      // Clear the previous game board
      clearGameBoard();
      
      // Set wing y to previous bird y
      wingY = birdY;
      
      // Update bird position
      if(btnPressed) {
        birdY--; 
      } else if(!oldBtnPressed) {
        birdY++;
      }
      
      // If bird goes off screen, game over
      if(birdY > 7 || birdY < 0) {
         pet.setState(GAME_OVER_STATE);
         break; 
      }
      
      // Update pipe position
      pipe1X--;
      if(pipe1X < 0) {
        pipe1 = generatePipe();
        pipe1X = 7; 
      }
      
      pipe2X--;
      if(pipe2X < 0) {
        pipe2 = generatePipe();
        pipe2X = 7; 
      }
      
      // Detect collision with either pipe
      // NB: No need to check for wing, collision as it's 
      // always one move behind the bird, so if the bird 
      // makes it, the wing is guaranteed to make it
      if(pipe1X == 2 && pipe1 & (0x80 >> birdY) ) {
         pet.setState(GAME_OVER_STATE);
         break;    
      }
      
      if(pipe2X == 2 && pipe2 & (0x80 >> birdY) ) {
         pet.setState(GAME_OVER_STATE);
         break;    
      }
      
      // Got through the pipe, so increase the score
      if(pipe1X == 0 || pipe2X == 0) {
        score++;
      }
    
      // Draw bird + wing
      drawPointToGameBoard(2, birdY, HIGH);
      drawPointToGameBoard(1, wingY, HIGH);
    
      // Draw pipes to screen
      drawPipeToGameBoard(pipe1, pipe1X);
      drawPipeToGameBoard(pipe2, pipe2X);
      
      // Draw game board to screen
      pet.drawImage(gameBoard);
    
      // Reset button pressed state
      oldBtnPressed = btnPressed;
      btnPressed = false;
      
      // Update speed based on score
      if(score > 50) {
        speed = 175;
      } else {
        speed = 200;
      }
    
      pet.setNextState(GAME_LOOP_STATE, speed);
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
      pet.setNextState(INTRO_STATE, 3000);
      break;
      
  }

}

void clearGameBoard() {
  for(int r=0;r<8;r++) {
    gameBoard[r] = 0x00;
  }
}

void drawPointToGameBoard(int x, int y, int val) {
  if(val == HIGH){
    gameBoard[y] |= (B10000000 >> x); 
  } else {
    gameBoard[y] ^= (B10000000 >> x);
  }
}

byte generatePipe() {
  unsigned int minGap = 3;
  unsigned int maxGap = 6;
  
  // Decrease possible hole size based on score
  if(score > 20) {
    minGap = 2;
    maxGap = 5;
  }
  
  byte gap = random(minGap, maxGap);
  byte punch = (1 << gap) - 1;
  byte slide = random(1, 8 - gap);
  return 0xff & ~(punch << slide);
}

void drawPipeToGameBoard(byte &pipe, int &pipeX) {
  if (pipeX >= 0 && pipeX < 8) {
    for (byte r=0; r<8; r++) {
      if(pipe & (0x80 >> r)) {
        gameBoard[r] |= (0x80 >> pipeX);
      }
    }
  } 
}
