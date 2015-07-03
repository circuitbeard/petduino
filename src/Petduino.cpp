/*
 *    Petduino.cpp - An Arduino library for interacting with a Petduino
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

#include "Petduino.h"

// Constructor
Petduino::Petduino() {
}

// Setup inputs, outputs, etc. Call this from main arduino setup() method
void Petduino::begin(){

  //Serial.begin(9600);

  // Setup outputs -----------------------------------

  // MAX72XX is in power-saving mode on startup, we have to do a wakeup call
  lc.shutdown(0, false);

  // set the brightness to low
  lc.setIntensity(0, 1);

  // Setup pins
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Setup inputs ------------------------------------
  pinMode(BTN1, INPUT);
  digitalWrite(BTN1, HIGH);
  pinMode(BTN2, INPUT);
  digitalWrite(BTN2, HIGH);
  pinMode(LDR, INPUT);
  pinMode(TMP, INPUT);

  // Flash LED to show ready
  setLed(true);
  delay(100);
  setLed(false);
  delay(100);
  setLed(true);
  delay(100);
  setLed(false);

}

// Update function to be called from main loop() method
void Petduino::update() {
  updateMelody();
  updateAnimation();
}

// Protected ===========================================

// Check to see if the state timer has expired and thus a change of state needs to occur
bool Petduino::stateIntervalExpired() {
  if (stateInterval > 0 && millis() - stateTimestamp >= stateInterval) {
    stateTimestamp = millis();
    return true;
  }
  return false;
}

// Read pin state with debounce
bool Petduino::debounce(uint8_t pin, int &state, int &lastState, unsigned long &debounceTimestamp) {
  bool changed = false;
  int reading = digitalRead(pin);
  if(reading != lastState) {
    debounceTimestamp = millis();
  }
  if((millis() - debounceTimestamp) > DEBOUNCE_TIME)
  {
    if (reading != state) {
      state = reading;
      changed = true;
    }
  }
  lastState = reading;
  return changed;
}

// Helper method to perform melody updates. Should be called in loop()
void Petduino::updateMelody() {
  if(melodyLength > 0){
    unsigned long ts = millis();
    if(ts - melodyTimestamp > melodyDurations[melodyIdx] + 5) { // Add 5 to give tone time between calls (Seems to need it)
      unsigned int nextIdx = melodyIdx + 1;
      if(nextIdx < melodyLength){
        melodyIdx = nextIdx;
        melodyTimestamp = ts;
        playMelodyTone(melodyIdx);
      } else {
        if(melodyLoopCounter < melodyLoopCount - 1) {
          melodyIdx = 0;
          melodyTimestamp = ts;
          melodyLoopCounter++;
          playMelodyTone(melodyIdx);
        } else {
          stopTone();
        }
      }
    }
  }
}

// Helper method to play a melody tone at the given index
void Petduino::playMelodyTone(unsigned long idx) {
  if(melodyFrequencies[idx] > 0){
    tone(BUZZER, melodyFrequencies[idx], melodyDurations[idx]);
  } else {
    noTone(BUZZER);
  }
}

// Helper method to perform animation updates. Should be called in loop()
void Petduino::updateAnimation() {
  if(animLength > 0) {
    unsigned long ts = millis();
    if(ts - animTimestamp > animFrameDurations[animIdx] + 5) { // Add 5 to give tone time between calls (Seems to need it)
      unsigned int nextIdx = animIdx + 1;
      if(nextIdx < animLength){
        animIdx = nextIdx;
        animTimestamp = ts;
        playAnimationFrame(animIdx);
      } else {
        if(animLoopCounter < animLoopCount - 1) {
          animIdx = 0;
          animTimestamp = ts;
          animLoopCounter++;
          playAnimationFrame(animIdx);
        } else {
          stopAnimation();
        }
      }
    }
  }
}

// Helper method to render an animation frame at the given index
void Petduino::playAnimationFrame(unsigned long idx) {
  drawImage(animFrames[idx]);
}

// Public ============================================

// Set the value of the Petduino LED
void Petduino::setLed(bool value) {
  digitalWrite(LED, value);
}

// Toggle the Petduino LED
void Petduino::toggleLed() {
  digitalWrite(LED, !digitalRead(LED));
}

// Get the Petduino LED state
bool Petduino::getLed() {
  return digitalRead(LED) == HIGH;
}

// Get's the current temperature reading in °C from the temperature sensor
float Petduino::getTemperature() {
  float temp = analogRead(TMP)*5/1024.0; // Convert reading to voltage
  temp = temp - 0.5; // Calibrate to 0°C
  temp = temp / 0.01; // Divide by mV per step
  return temp;
}

// Get's the current light level reading from the ldr sensor
int Petduino::getLightLevel() {
  return analogRead(LDR);
}

// Returns true if button 1 is pressed
bool Petduino::isBtn1Pressed() {
  bool changed = debounce(BTN1, btn1State, btn1LastState, btn1DebounceTimestamp);
  return btn1State == LOW && changed;
}

// Returns true while button 1 is held down
bool Petduino::isBtn1Held() {
  debounce(BTN1, btn1State, btn1LastState, btn1DebounceTimestamp);
  return btn1State == LOW;
}

// Returns true if button 2 is pressed
bool Petduino::isBtn2Pressed() {
  bool changed = debounce(BTN2, btn2State, btn2LastState, btn2DebounceTimestamp);
  return btn2State == LOW && changed;
}

// Returns true while button 2 is held down
bool Petduino::isBtn2Held() {
  debounce(BTN2, btn2State, btn2LastState, btn2DebounceTimestamp);
  return btn2State == LOW;
}

// Plays a tone at the given frequency for the specified duration
void Petduino::playTone(unsigned int frequency, unsigned long duration) {
  stopTone();
  tone(BUZZER, frequency, duration);
}

// Plays a melody specified by the freq / duration arrays
void Petduino::playMelody(unsigned int frequencies[], unsigned long durations[], unsigned long length, unsigned long loopCount) {
  stopTone();
  melodyFrequencies = frequencies;
  melodyDurations = durations;
  melodyLength = length;
  melodyLoopCount = loopCount;
  melodyTimestamp = millis();
  playMelodyTone(melodyIdx);
}

// Stops the current tone
void Petduino::stopTone() {
  melodyLength = 0;
  melodyLoopCount = 0;
  melodyIdx = 0;
  melodyLoopCounter = 0;
  noTone(BUZZER);
}

// Sets the brightness of the screen from 0 to 8
void Petduino::setScreenBrightness(unsigned int level) {
  lc.setIntensity(0, level);
}

// Turns on all pixels on the screen
void Petduino::fillScreen() {
  byte b = B11111111;
  for (int r=0; r<8; r++)
  {
    drawRow(r, b);
  }
}

// Turns off all pixels on the screen
void Petduino::clearScreen() {
  lc.clearDisplay(0);
}

// Draw a row to the display
void Petduino::drawRow(int row, byte val) {
  // Because the screen is rotated, fill the oposite row and use setColumn not setRow
  row = abs(row - 7);
  lc.setColumn(0, row, val);
}

// Draw an image to the screen
void Petduino::drawImage(byte img[]) {
  for(int r=0; r<8; r++) {
    drawRow(r, img[r]);
  }
}

// Draw an animation seuquence to the screen
void Petduino::playAnimation(byte frames[][8], unsigned long frameDurations[], unsigned long length, unsigned long loopCount) {
  stopAnimation();
  animFrames = frames;
  animFrameDurations = frameDurations;
  animLength = length;
  animLoopCount = loopCount;
  animTimestamp = millis();
  playAnimationFrame(animIdx);
}

// Stops the current animation
void Petduino::stopAnimation() {
  animLength = 0;
  animLoopCount = 0;
  animIdx = 0;
  animLoopCounter = 0;
}

// Gets the current state, switching state if next action should occur
unsigned int Petduino::getState() {
  if(currentState == WAIT_INTERVAL_STATE && stateIntervalExpired()) {
    setState(nextState);
  }
  return currentState;
}

// Sets the current state without waiting, canceling any previously scheduled states
void Petduino::setState(unsigned int state) {
  setNextState(state, 0);
}

// Lines up the next state to move to after the given interval
void Petduino::setNextState(unsigned int state, unsigned long interval) {
  if(interval == 0) {
    currentState = state; // Apply the state imediatly
    stateInterval = 0; // Reset state interval
  } else {
    currentState = WAIT_INTERVAL_STATE; // Set to wait mode
    nextState = state; // Store next state
    stateInterval = interval; // Store interval to wait
    stateTimestamp = millis();  // Store current time
  }
}

// Wait indefinately
void Petduino::wait() {
  setState(WAIT_STATE);
}
