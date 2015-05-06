/*
 *    Petduino.h - An Arduino library for interacting with a Petduino
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

#ifndef Petduino_h
#define Petduino_h
#include <LedControl.h>

class Petduino {

    private :

      // Keywords
      #define DIN    12
      #define CS     11
      #define CLK    10
      #define LED    2
      #define BUZZER 3
      #define TMP    16 //A2
      #define BTN1   14 //A0
      #define BTN2   15 //A1
      #define LDR    17 //A3

      #define DEBOUNCE_TIME  10 //ms

      #define WAIT_INTERVAL_STATE 65535 //UInt Max
      #define WAIT_STATE 65534 //UInt Max - 1

      // Variables
      LedControl lc = LedControl(DIN, CLK, CS, 1);

      int btn1State = HIGH;
      int btn1LastState = HIGH;
      unsigned long btn1DebounceTimestamp;

      int btn2State = HIGH;
      int btn2LastState = HIGH;
      unsigned long btn2DebounceTimestamp;

      unsigned int* melodyFrequencies;
      unsigned long* melodyDurations;
      unsigned long melodyLength;
      unsigned long melodyLoopCount;
      unsigned long melodyIdx;
      unsigned long melodyTimestamp;
      unsigned long melodyLoopCounter;

      byte (*animFrames)[8];
      unsigned long* animFrameDurations;
      unsigned long animLength;
      unsigned long animLoopCount;
      unsigned long animIdx;
      unsigned long animTimestamp;
      unsigned long animLoopCounter;

      unsigned int currentState;
      unsigned int nextState;
      unsigned long stateTimestamp;
      unsigned long stateInterval;

      // Methods
      bool stateIntervalExpired();
      bool debounce(uint8_t pin, int &state, int &lastState, unsigned long &debounceTimestamp);

      void updateMelody();
      void playMelodyTone(unsigned long idx);

      void updateAnimation();
      void playAnimationFrame(unsigned long idx);

    public:

      Petduino();

      // Methods
      void begin();
      void update();

      void setLed(bool value);
      void toggleLed();

      float getTemperature();
      int getLightLevel();

      bool isBtn1Pressed();
      bool isBtn1Held();
      bool isBtn2Pressed();
      bool isBtn2Held();

      void playTone(unsigned int frequency, unsigned long duration);
      void playMelody(unsigned int frequencies[], unsigned long durations[], unsigned long length, unsigned long loopCount);
      void stopTone();

      void setScreenBrightness(unsigned int level);
      void fillScreen();
      void clearScreen();
      void drawRow(int row, byte val);
      void drawImage(byte img[]);
      void playAnimation(byte frames[][8], unsigned long frameDurations[], unsigned long length, unsigned long loopCount);
      void stopAnimation();

      unsigned int getState();
      void setState(unsigned int state);
      void setNextState(unsigned int state, unsigned long interval);
      void wait();

};

#endif  //Petduino.h
