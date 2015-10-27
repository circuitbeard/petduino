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

#define SCREEN_WIDTH 8
#define SCREEN_HEIGHT 8

#define CHAR_HEIGHT SCREEN_HEIGHT
#define BUFFER_SIZE CHAR_HEIGHT*2
#define SCROLL_SPEED 75
#define MIN_CHAR 0x20
#define MAX_CHAR 0x7f 

const unsigned char scrollText[] PROGMEM = {"CB 2015\0"};
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
  
  Serial.begin(9600);
  
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
      scrollLen = 0;
      scrollCount = 0;
      
      // Clear any existing buffer
      clearBuffer();
      
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

        // Scroll char into view
        pet.setState(LOOP_STATE);

      } else {
        
        // End of message
        scrollLen = SCREEN_WIDTH;
        scrollCount = 0;
        
        // Scroll buffer till clear
        pet.setState(LOOP_STATE);
        
      }

      break;
    case LOOP_STATE:

      // Keep scrolling untill incoming char is completely onscreen
      if(scrollCount < scrollLen){
        
        // Shift the buffer along 1px
        scrollBufferLeftOne();
        
        // Draw the current buffer
        drawBuffer();
        
        // Increment the scroll counter
        scrollCount++;
        
        // Loop
        pet.setNextState(LOOP_STATE, SCROLL_SPEED);
        
      } else {
        
        // Check to see if message has ended
        if(nextChar != 0) {
          // Load next char
          pet.setState(LOAD_CHAR_STATE);
        } else {
          // No more chars and buffer clear, so start again
          pet.setState(INIT_STATE);
        }
        
      }

      break;
  }

}

unsigned int loadCharIntoBuffer(unsigned int ascii){
  
  if (ascii >= MIN_CHAR && ascii <= MAX_CHAR){ 
    for (int a=0; a<CHAR_HEIGHT; a++) {     
      
      // Get char row data
      unsigned long c = pgm_read_byte_near(font + ((ascii - 0x20) * (CHAR_HEIGHT+1)) + a); 
      
      // Because unsigned longs are 32 bits, shift all the bits left 
      // so they are just off screen otherwise there is a long pause 
      // before anything displays
      c = c<<(31-SCREEN_WIDTH); 
      
      // Bitwise OR character row data onto end
      charBuffer[a*2] = charBuffer[a*2] | c; 
    }
    
    // Get and return width of char so we know how long to scroll for
    return pgm_read_byte_near(font + ((ascii - 0x20) * (CHAR_HEIGHT+1)) + CHAR_HEIGHT); 
    
  } else {
    
    // Char not in range so return 0 to move straight onto next char
    return 0;
  }
  
}

void scrollBufferLeftOne(){
  
  for (int a=0;a<CHAR_HEIGHT;a++){
    
    // Scroll offscreen data left one
    unsigned long x = charBuffer[a*2];     // Get offscreen char row data
    byte b = bitRead(x,31);                // Get left most bit (the bit that will be pushed into view)
    x = x<<1;                              // Bitwise shift the row left 1
    charBuffer[a*2] = x;                   // Store new offscreen row data value
    
    // Scroll onscreen data left one
    x = charBuffer[(a*2)+1];               // Get onscreen char row data
    x = x << 1;                            // Bitwise shift the row left 1
    bitWrite(x,0,b);                       // Append on the incoming bit
    charBuffer[(a*2)+1] = x;               // Store new onscreen row data value
    
  }
  
}

void drawBuffer(){
  for (int a=0; a<CHAR_HEIGHT; a++){
    pet.drawRow(a, charBuffer[(a*2)+1]);  
  }
}

void clearBuffer(){
  for (int a=0; a<BUFFER_SIZE; a++){
    charBuffer[a] = 0;
  }
}
