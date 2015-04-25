/*
 *    LdrExample.ino - LDR example using the Petduino library
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

#define FRAMECOUNT 8
byte frames[FRAMECOUNT][8] = {
  {
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B10000000
  },
  {
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B01000000,
    B11000000
  },
  {
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00100000,
    B01100000,
    B11100000
  },
  {
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00010000,
    B00110000,
    B01110000,
    B11110000
  },
  {
    B00000000,
    B00000000,
    B00000000,
    B00001000,
    B00011000,
    B00111000,
    B01111000,
    B11111000
  },
  {
    B00000000,
    B00000000,
    B00000100,
    B00001100,
    B00011100,
    B00111100,
    B01111100,
    B11111100
  },
  {
    B00000000,
    B00000010,
    B00000110,
    B00001110,
    B00011110,
    B00111110,
    B01111110,
    B11111110
  },
  {
    B00000001,
    B00000011,
    B00000111,
    B00001111,
    B00011111,
    B00111111,
    B01111111,
    B11111111
  }
};

unsigned int currentFrame;

Petduino pet = Petduino();

void setup() {
  
  // Setup Petduino
  pet.begin();
  
}

void loop() {
  
  // Call pet loop
  pet.loop();
  
  // Map reading to a frame
  currentFrame = map(pet.getLightLevel(), 0, 1023, 0, FRAMECOUNT);
  
  // Draw the frame
  pet.drawImage(frames[currentFrame]);
  
  // Wait a split second and repeat
  delay(200);

}
