/*
 *    ButtonExample.ino - Button example using the Petduino library
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

byte question[8] = {
  B01111110,
  B11111111,
  B11000011,
  B00001110,
  B00011100,
  B00011000,
  B00000000,
  B00011000
};

byte leftArrow[8] = {
  B00010000,
  B00110000,
  B01110000,
  B11111111,
  B11111111,
  B01110000,
  B00110000,
  B00010000
};

byte rightArrow[8] = {
  B00001000,
  B00001100,
  B00001110,
  B11111111,
  B11111111,
  B00001110,
  B00001100,
  B00001000
};

#define QUESTION_STATE 0
#define LEFT_STATE 1
#define RIGHT_STATE 2

Petduino pet = Petduino();

void setup() {

  // Setup Petduino
  pet.begin();

  // Set initial state
  pet.setState(QUESTION_STATE);

}

void loop() {

  // Update pet
  pet.update();

  // Check button 1
  if(pet.isBtn1Pressed()) {
    pet.setState(LEFT_STATE);
  }

  // Check button 2
  if(pet.isBtn2Pressed()) {
    pet.setState(RIGHT_STATE);
  }

  // Handle current state
  switch(pet.getState()){
    case QUESTION_STATE:
      pet.drawImage(question);
      pet.wait();
      break;
    case LEFT_STATE:
      pet.drawImage(leftArrow);
      pet.setNextState(QUESTION_STATE, 1000);
      break;
    case RIGHT_STATE:
      pet.drawImage(rightArrow);
      pet.setNextState(QUESTION_STATE, 1000);
      break;
  }
}
