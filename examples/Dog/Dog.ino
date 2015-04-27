/*
 *    Dog.ino - Dog example using state and the Petduino library
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

byte dog[8]={
  B00000000,
  B01000100,
  B01100110,
  B00000000,
  B01111110,
  B00111100,
  B10000001,
  B01111110
};

byte dogBlink[8]={
  B00000000,
  B00000000,
  B01100110,
  B00000000,
  B01111110,
  B00111100,
  B10000001,
  B01111110
};

#define DOG_SPEAK_FRAMES 4
byte dogSpeakF[DOG_SPEAK_FRAMES][8]= {
  {
    B00000000,
    B01000100,
    B01100110,
    B00000000,
    B01111110,
    B00111100,
    B10011001,
    B01111110
  },
  {
    B00000000,
    B01000100,
    B01100110,
    B00000000,
    B01111110,
    B00111100,
    B11111111,
    B01111110
  },
  {
    B00000000,
    B01000100,
    B01100110,
    B00000000,
    B01111110,
    B00111100,
    B10011001,
    B01111110
  },
  {
    B00000000,
    B01000100,
    B01100110,
    B00000000,
    B01111110,
    B00111100,
    B10000001,
    B01111110
  }
};
unsigned long dogSpeakD[DOG_SPEAK_FRAMES] = { 100, 100, 100, 100 };

#define DOG_SPEAK_TONES 11
unsigned int dogSpeakToneF[DOG_SPEAK_TONES] = { 50, 100, 400, 220, 300, 50, 250, 150, 70, 200, 100 };
unsigned long dogSpeakToneD[DOG_SPEAK_TONES] = { 20, 50, 20, 10, 100, 50, 40, 20, 80, 20, 30 };

#define DOG_LOOK_FRAMES 4
byte dogLookF[DOG_LOOK_FRAMES][8]= {
  {
    B00000000,
    B00100010,
    B01100110,
    B00000000,
    B01111110,
    B00111100,
    B10000001,
    B01111110
  },
  {
    B00000000,
    B01100110,
    B00100010,
    B00000000,
    B01111110,
    B00111100,
    B10000001,
    B01111110
  },
  {
    B00000000,
    B01100110,
    B01000100,
    B00000000,
    B01111110,
    B00111100,
    B10000001,
    B01111110
  },
  {
    B00000000,
    B01000100,
    B01100110,
    B00000000,
    B01111110,
    B00111100,
    B10000001,
    B01111110
  }
};
unsigned long dogLookD[DOG_LOOK_FRAMES] = { 100, 100, 100, 100 };

#define DEFAULT_STATE 0
#define BLINK_STATE 1
#define LOOK_STATE 2
#define SPEAK_STATE 3

Petduino pet = Petduino();

void setup() {

  // Setup Petduino
  pet.begin();

  // Set initial state
  pet.setState(DEFAULT_STATE);

  // Initialize random seed
  randomSeed(analogRead(0));

}

void loop() {

  // Update pet
  pet.update();

  // Check buttons
  if(pet.isBtn1Pressed()){
    pet.setState(LOOK_STATE);
  }

  if(pet.isBtn2Pressed()){
    pet.setState(SPEAK_STATE);
  }

  // Update display based on current state
  switch(pet.getState()){

    case DEFAULT_STATE:
      if (random(0, 5) == 0) {
        pet.setState(BLINK_STATE); // Random blink
      } else if(random(0, 10) == 0) {
        pet.setState(SPEAK_STATE); // Random speak
      } else if(random(0, 5) == 0) {
        pet.setState(LOOK_STATE); // Random look
      } else {
        pet.drawImage(dog);
        pet.setNextState(DEFAULT_STATE, 3000);
      }
      break;

    case BLINK_STATE:
      pet.drawImage(dogBlink);
      pet.setNextState(DEFAULT_STATE, 100);
      break;

    case LOOK_STATE:
      pet.playAnimation(dogLookF, dogLookD, DOG_LOOK_FRAMES, 2);
      pet.setNextState(DEFAULT_STATE, 2000);
      break;

    case SPEAK_STATE:
      pet.playAnimation(dogSpeakF, dogSpeakD, DOG_SPEAK_FRAMES, 4);
      pet.playMelody(dogSpeakToneF, dogSpeakToneD, DOG_SPEAK_TONES, 3);
      pet.setNextState(DEFAULT_STATE, 2000);
      break;
  }

}
