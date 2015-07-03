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

    protected:

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
      virtual void begin();
      virtual void update();

      virtual void setLed(bool value);
      virtual void toggleLed();
      virtual bool getLed();

      virtual float getTemperature();
      virtual int getLightLevel();

      virtual bool isBtn1Pressed();
      virtual bool isBtn1Held();
      virtual bool isBtn2Pressed();
      virtual bool isBtn2Held();

      virtual void playTone(unsigned int frequency, unsigned long duration);
      virtual void playMelody(unsigned int frequencies[], unsigned long durations[], unsigned long length, unsigned long loopCount);
      virtual void stopTone();

      virtual void setScreenBrightness(unsigned int level);
      virtual void fillScreen();
      virtual void clearScreen();
      virtual void drawRow(int row, byte val);
      virtual void drawImage(byte img[]);
      virtual void playAnimation(byte frames[][8], unsigned long frameDurations[], unsigned long length, unsigned long loopCount);
      virtual void stopAnimation();

      virtual unsigned int getState();
      virtual void setState(unsigned int state);
      virtual void setNextState(unsigned int state, unsigned long interval);
      virtual void wait();

};

#endif  //Petduino.h
