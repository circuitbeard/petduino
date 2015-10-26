/*
 *    ScrollingTextState.ino - Scrolling text example using the Petduino library
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

#include <avr/pgmspace.h> 
#include <LedControl.h>
#include <Petduino.h>
#include "font_bold.h"

#define CHAR_HEIGHT 8
#define BUFFER_SIZE CHAR_HEIGHT*2
#define SCROLL_SPEED 75
#define MIN_CHAR 0x20
#define MAX_CHAR 0x7f 

const unsigned char scrollText[] PROGMEM = {"HELLO WORLD #CIRCUITBEARD 2015  \0"}; // Make sure you have enough space on the end to fill the buffer and ensure all chars scroll across screen
unsigned long charBuffer[BUFFER_SIZE] = {0};

unsigned int charCounter = 0;
unsigned int nextChar = 0;

unsigned int scrollLen = 0;
unsigned int scrollCount = 0;

#define INIT_STATE 0
#define LOAD_CHAR_STATE 1
#define LOOP_STATE 2

Petduino pet = Petduino();

void setup() {
  
  // Setup Petduino
  pet.begin();

  // Set initial state
  pet.setState(INIT_STATE);

}

void loop() {

  // Call pet loop
  pet.update();

  // Update based on state
  switch(pet.getState()) {

    case INIT_STATE:

      // Reset vars
      charCounter = 0;
      nextChar = 0;

      // Start loading text into buffer
      pet.setState(LOAD_CHAR_STATE);
      break;

    case LOAD_CHAR_STATE:

      // Grab the next character
      nextChar =  pgm_read_byte_near(scrollText + charCounter);

      if (nextChar != 0){

        // Load char into buffer and reset scroll vars
        scrollLen = loadCharIntoBuffer(nextChar);
        scrollCount = 0;

        // Increment char counter
        charCounter++;

        // Draw current buffer
        pet.setState(LOOP_STATE);

      } else {

        // Finished so start again
        pet.setState(INIT_STATE);

      }

      break;
    case LOOP_STATE:

      if(scrollCount < scrollLen){
        
        // Shift the buffer along 1px
        rotateBuffer();
        
        // Draw the current buffer
        drawBuffer();
        
        // Increment the scroll counter
        scrollCount++;
        
        // Loop
        pet.setNextState(LOOP_STATE, SCROLL_SPEED);
        
      } else {
        
        // Load next char
        pet.setState(LOAD_CHAR_STATE);
        
      }

      break;
  }

}

// Load character into buffer
unsigned int loadCharIntoBuffer(unsigned int ascii){
  if (ascii >= MIN_CHAR && ascii <= MAX_CHAR){ 
    for (int a=0; a<CHAR_HEIGHT; a++) {                      
        unsigned long c = pgm_read_byte_near(font + ((ascii - 0x20) * (CHAR_HEIGHT+1)) + a);     // Index into character table to get row data
        charBuffer[a*2] = charBuffer[a*2] | c; // Bitwise OR character onto end
    }
    return pgm_read_byte_near(font + ((ascii - 0x20) * (CHAR_HEIGHT+1)) + CHAR_HEIGHT);     // Index into character table for kerning data
  } else {
    return 0;
  }
}

// Rotate the buffer
void rotateBuffer(){
  for (int a=0;a<CHAR_HEIGHT;a++){         // Loop each row in char
    unsigned long x = charBuffer[a*2];     // Get low buffer entry
    byte b = bitRead(x,31);                // Copy high order bit that gets lost in rotation
    x = x<<1;                              // Rotate left one bit
    charBuffer[a*2] = x;                   // Store new low buffer  
    x = charBuffer[a*2+1];                 // Get high buffer entry
    x = x<<1;                              // Rotate left one bit
    bitWrite(x,0,b);                       // Store saved bit
    charBuffer[a*2+1] = x;                 // Store new high buffer
  }
}

// Display Buffer on screen
void drawBuffer(){
  for (int a=0; a<CHAR_HEIGHT; a++){        // Loop each row in char
    pet.drawRow(a, charBuffer[a*2+1]);      // Send row to screen
  }
}
