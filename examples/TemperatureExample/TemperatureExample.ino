/*
 *    TemperatureExample.ino - Temperature example using the Petduino library
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

#define NUMBERCOUNT 10
byte numbers[NUMBERCOUNT][8] = {
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

float temp;
unsigned int ones, tens;
byte num[8];

Petduino pet = Petduino();

void setup() {
  
  // Setup Petduino
  pet.begin();
  
}

void loop() {
  
  // Call pet loop
  pet.loop();
  
  // Get the temperature
  temp = pet.getTemperature();
  
  // Split the digits
  tens = temp/10;
  ones = temp-tens*10;
    
  // Generate number graphic
  for(int b = 0; b < 8; b++){
    num[b] = numbers[tens][b] | numbers[ones][b] >> 4;
  }
  
  // Draw the number to the display
  pet.drawImage(num);
  delay(500);

}
