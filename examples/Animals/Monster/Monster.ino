/*
 *    Monster.ino - Monster example using state and the Petduino library
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

byte monster[8]={
  B00000000,
  B01100110,
  B01100110,
  B01000010,
  B00011000,
  B01000010,
  B01000010,
  B11111111
};

byte monsterBlink[8]={
  B00000000,
  B00000000,
  B01100110,
  B00000000,
  B00011000,
  B01000010,
  B01000010,
  B11111111
};

#define MONSTER_SPEAK_FRAMES 4
byte monsterSpeakF[MONSTER_SPEAK_FRAMES][8]= {
  {
    B00000000,
    B01100110,
    B01100110,
    B01000010,
    B00011000,
    B01000010,
    B01111110,
    B11111111
  },
  {
    B00000000,
    B01100110,
    B01100110,
    B01000010,
    B00011000,
    B01111110,
    B01000010,
    B11111111
  },
  {
    B00000000,
    B01100110,
    B01100110,
    B01000010,
    B00011000,
    B01000010,
    B01111110,
    B11111111
  },
  {
    B00000000,
    B01100110,
    B01100110,
    B01000010,
    B00011000,
    B01000010,
    B01000010,
    B11111111
  }
};
unsigned long monsterSpeakD[MONSTER_SPEAK_FRAMES] = { 60, 800, 60, 100 };

#define MONSTER_SPEAK_TONES 4
unsigned int monsterSpeakToneF[MONSTER_SPEAK_TONES] = { 40, 50, 40, 0 };
unsigned long monsterSpeakToneD[MONSTER_SPEAK_TONES] = { 60, 800, 60, 100 };

#define MONSTER_LOOK_FRAMES 6
byte monsterLookF[MONSTER_LOOK_FRAMES][8]= {
  {
    B00000000,
    B01100110,
    B01000010,
    B01100110,
    B00011000,
    B01000010,
    B01000010,
    B11111111
  },
  {
    B00000000,
    B01000010,
    B01100110,
    B01100110,
    B00011000,
    B01000010,
    B01000010,
    B11111111
  },
  {
    B00000000,
    B00100100,
    B01100110,
    B01100110,
    B00011000,
    B01000010,
    B01000010,
    B11111111
  },
  {
    B00000000,
    B01100110,
    B00100100,
    B01100110,
    B00011000,
    B01000010,
    B01000010,
    B11111111
  },
  {
    B00000000,
    B01100110,
    B01100110,
    B00100100,
    B00011000,
    B01000010,
    B01000010,
    B11111111
  },
  {
    B00000000,
    B01100110,
    B01100110,
    B01000010,
    B00011000,
    B01000010,
    B01000010,
    B11111111
  }
};
unsigned long monsterLookD[MONSTER_LOOK_FRAMES] = { 100, 100, 100, 100, 100, 100 };

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
        pet.drawImage(monster);
        pet.setNextState(DEFAULT_STATE, 3000);
      }
      break;

    case BLINK_STATE:
      pet.drawImage(monsterBlink);
      pet.setNextState(DEFAULT_STATE, 100);
      break;

    case LOOK_STATE:
      pet.playAnimation(monsterLookF, monsterLookD, MONSTER_LOOK_FRAMES, 2);
      pet.setNextState(DEFAULT_STATE, 2000);
      break;

    case SPEAK_STATE:
      pet.playAnimation(monsterSpeakF, monsterSpeakD, MONSTER_SPEAK_FRAMES, 1);
      pet.playMelody(monsterSpeakToneF, monsterSpeakToneD, MONSTER_SPEAK_TONES, 1);
      pet.setNextState(DEFAULT_STATE, 2000);
      break;
  }

}
