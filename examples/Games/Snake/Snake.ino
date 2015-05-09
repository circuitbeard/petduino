/*
 *    Snake.ino - Snake game example using the Petduino library
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

byte intro[8]={
  B00000000,
  B01111010,
  B01000000,
  B01111110,
  B00000010,
  B00000010,
  B01111110,
  B00000000
};

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

unsigned int goalX = 0;
unsigned int goalY = 0;
int headX = 0;
int headY = 0;
int snakeLength = 2;
int score = 0;
int dir = 0;
int speed = 700;

#define MAX_HISTORY 64 // 8x8
int historyX[MAX_HISTORY];
int historyY[MAX_HISTORY];
int historyF = 0;

bool gameStarted = false;
byte gameBoard[8];

unsigned int scoreOnes;
unsigned int scoreTens;

#define DIR_UP 0 
#define DIR_RIGHT 1
#define DIR_DOWN 2
#define DIR_LEFT 3
#define DIR_MAX 3

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
      dir = wrap(dir - 1, DIR_MAX + 1);
    }
  }
  
  if(pet.isBtn2Pressed()){
    if(!gameStarted){
      pet.setState(GAME_START_STATE); 
    } else {
      dir = wrap(dir + 1, DIR_MAX + 1);
    }
  }

  // Update based on state
  switch(pet.getState()) {
    
    case INTRO_STATE:
      pet.drawImage(intro);
      pet.wait(); // Wait for button press
      break;
      
    case GAME_START_STATE:
    
      // Flag game started
      gameStarted = true;
    
      // Clear any previous history
      clearHistory();
      
      // Reset variables
      score = 0;
      snakeLength = 2;
      
      // Start in bottom left corner
      headX = 0;
      headY = 6;
      
      // Start moving right
      dir = DIR_RIGHT;
      
      // Draw initial snake
      appendHistoryPoint(headX, headY);
      for(int i=0;i<snakeLength-1;i++){
        moveHead(dir); 
        appendHistoryPoint(headX, headY);
      }
      
      // Setup initial goal
      goalX = random(0, 8);
      goalY = random(0, 8);
      
      drawPointToGameBoard(goalX, goalY, HIGH);
      
      // Start game
      pet.setState(GAME_LOOP_STATE);
      break;
      
    case GAME_LOOP_STATE:

      // Clear the gameboard
      clearGameBoard();
      
      // Update snake head
      moveHead(dir); 
      
      // Append new snake head to history
      appendHistoryPoint(headX, headY);
      
      // Check for collision
      if(detectSnakeCollision()){
        pet.setState(GAME_OVER_STATE); 
        break;
      }
      
      // Check for goal capture
      if(headX == goalX && headY == goalY) {
        score++;
        snakeLength++;
        goalX = random(0, 8);
        goalY = random(0, 8); 
      }
      
      // Draw snake to game board
      drawSnakeToGameBoard();
      
      // Draw goal to the gameboard
      drawPointToGameBoard(goalX, goalY, HIGH);
      
      // Draw gameboard to screen
      pet.drawImage(gameBoard);
      
      // Update speed based on score
      if(score > 20) {
        speed = 500;
      } else if (score > 15) {
        speed = 550;
      } else if (score > 10) {
        speed = 600;
      } else if (score > 5) {
        speed = 650;
      } else {
        speed = 700;
      }
      
      // Loop
      pet.setNextState(GAME_LOOP_STATE, speed);
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

bool detectSnakeCollision() {
  for(int i=1;i<snakeLength;i++) { // Start at 1 because we dont count the head
    int hIdx = wrap(historyF - i, MAX_HISTORY);
    if(historyX[hIdx] == headX && historyY[hIdx] == headY) {
      return true; 
    }
  }
  return false;
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

void drawSnakeToGameBoard() {
  for(int i=0;i<snakeLength;i++) {
    int hIdx = wrap(historyF - i, MAX_HISTORY);
    drawPointToGameBoard(historyX[hIdx], historyY[hIdx], HIGH); 
  }
}

void clearHistory() {
  historyF = 0;
  for(int i=0; i<MAX_HISTORY; i++)
  {
    historyX[i] = 0;
    historyY[i] = 0;
  }  
}

void appendHistoryPoint(int x, int y) {
  historyF = wrap(historyF + 1, MAX_HISTORY);
  historyX[historyF] = x;
  historyY[historyF] = y;
}

void moveHead(int dir) {
  switch(dir) {
    case DIR_UP:
      headY = wrap(headY - 1, 8);
      break;
    case DIR_RIGHT:
      headX = wrap(headX + 1, 8);
      break;
    case DIR_DOWN:
      headY = wrap(headY + 1, 8);
      break;
    case DIR_LEFT:
      headX = wrap(headX - 1, 8);
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
