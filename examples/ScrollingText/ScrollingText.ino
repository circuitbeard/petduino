/*
 *    ScrollingText.ino - Scrolling text example using the Petduino library
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

const unsigned char scrollText[] PROGMEM = {"  HELLO WORLD!  #CIRCUITBEARD 2015  \0"};
unsigned long charBuffer [BUFFER_SIZE] = {0};

Petduino pet = Petduino();

void setup() {
  
  // Setup Petduino
  pet.begin();

}

void loop() {

  // Call pet loop
  pet.update();

  // Scroll message
  scrollMessage(scrollText);

}

// Scroll Message
void scrollMessage(const unsigned char *messageString) {
    int counter = 0;
    int myChar=0;
    do {
        // read back a char
        myChar =  pgm_read_byte_near(messageString + counter);
        if (myChar != 0){
            loadCharIntoBuffer(myChar);
        }
        counter++;
    }
    while (myChar != 0);
}

// Load character into buffer
void loadCharIntoBuffer(int ascii){
  if (ascii >= MIN_CHAR && ascii <= MAX_CHAR){ 
      for (int a=0; a<CHAR_HEIGHT; a++) {                      
          unsigned long c = pgm_read_byte_near(font + ((ascii - 0x20) * (CHAR_HEIGHT+1)) + a);     // Index into character table to get row data
          unsigned long x = charBuffer[a*2];     // Load current buffer value
          x = x | c;                             // OR the new character onto end of current
          charBuffer[a*2] = x;                   // Store back in buffer
      }
      byte count = pgm_read_byte_near(font +((ascii - 0x20) * (CHAR_HEIGHT+1)) + CHAR_HEIGHT);     // Index into character table for kerning data
      for (byte x=0; x<count;x++){
          rotateBuffer();
          drawBuffer();
          delay(SCROLL_SPEED);
      }
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
