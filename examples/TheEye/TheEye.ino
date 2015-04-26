#include <LedControl.h>
#include <Petduino.h>

byte eye[8]={
  B00111100,
  B01111110,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B01111110,
  B00111100
};

byte pupil = B00011000;

int currentPupilX = 0;
int currentPupilY = 0;
int targetPupilX = 0;
int targetPupilY = 0;
int blinkState = 0;

#define PUPIL_MIN_OFFSET -2
#define PUPIL_MAX_OFFSET  2
#define PUPIL_SPEED 50

#define BLINK_SPEED 20
#define SPIN_SPEED 10
#define SPIN_COUNT 3

#define DEFAULT_STATE 0
#define PUPIL_ANIM_STATE 100
#define PUPIL_ANIM_LOOP_STATE 101
#define BLINK_ANIM_STATE 200
#define BLINK_ANIM_OPEN_LOOP_STATE 201
#define BLINK_ANIM_CLOSE_LOOP_STATE 202
#define HORIZONTAL_SPIN_STATE 300
#define ROUND_SPIN_STATE 400

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

  // Update the pet
  pet.update();

  // Check buttons
  if(pet.isBtn1Pressed()){
    pet.setState(HORIZONTAL_SPIN_STATE);
  }

  if(pet.isBtn2Pressed()){
    pet.setState(ROUND_SPIN_STATE);
  }

  // Update display based on current state
  switch(pet.getState()){

    case DEFAULT_STATE:
      if (random(0, 5) == 0) { 
        pet.setNextState(BLINK_ANIM_STATE, 500); // Randomly blink
      } else {
        drawEye();
        pet.setNextState(PUPIL_ANIM_STATE, random(5, 7) * 500); // Move pupil
      }
      break;
      
    case PUPIL_ANIM_STATE:
      // Choose random pupil position
      targetPupilX = random(PUPIL_MIN_OFFSET, PUPIL_MAX_OFFSET + 1);
      targetPupilY = random(PUPIL_MIN_OFFSET, PUPIL_MAX_OFFSET + 1);
      pet.setState(PUPIL_ANIM_LOOP_STATE);
      // Don't break so we fall throuh to first loop straight away
    case PUPIL_ANIM_LOOP_STATE:
      if(currentPupilX != targetPupilX || currentPupilY != targetPupilY){
        updatePupilAnim();
        pet.setNextState(PUPIL_ANIM_LOOP_STATE, PUPIL_SPEED);
      } else {
        pet.setState(DEFAULT_STATE);
      }
      break;
      
    case BLINK_ANIM_STATE:
      blinkState = 0;
      // Don't break so we fall throuh to first loop straight away
    case BLINK_ANIM_CLOSE_LOOP_STATE:
      if(blinkState < 4) {
        updateEyeAnim();
        blinkState++;
        pet.setNextState(BLINK_ANIM_CLOSE_LOOP_STATE, BLINK_SPEED);
      } else {
        pet.setState(BLINK_ANIM_OPEN_LOOP_STATE);
      }
      break;
    case BLINK_ANIM_OPEN_LOOP_STATE:
      if(blinkState > 0) {
        blinkState--;
        updateEyeAnim();
        pet.setNextState(BLINK_ANIM_OPEN_LOOP_STATE, BLINK_SPEED);
      } else {
        drawEye();
        pet.setState(DEFAULT_STATE);
      }
      break;
      
    case HORIZONTAL_SPIN_STATE:
      horizontalSpin();
      pet.setNextState(DEFAULT_STATE, 1000);
      break;
      
    case ROUND_SPIN_STATE:
      roundSpin();
      pet.setNextState(DEFAULT_STATE, 1000);
      break;
  }

}

// Spin the eye around in a circle [BLOCKING]
void roundSpin() {
  
  // Force eye open
  while(blinkState > 0) {
    blinkState--;
    updateEyeAnim();
    delay(BLINK_SPEED);
  }
  
  // Position the pupil
  targetPupilX = 2;
  targetPupilY = 0;
  
  while(currentPupilX != targetPupilX || currentPupilY != targetPupilY) {
     updatePupilAnim();
     delay(PUPIL_SPEED);
  }
  
  delay(200);
  
  // Perform spin
  for (int t=0; t<SPIN_COUNT; t++)
  {
    moveEye(2,-1);  delay(SPIN_SPEED);
    moveEye(1,-2);  delay(SPIN_SPEED);
    moveEye(0,-2);  delay(SPIN_SPEED);
    moveEye(-1,-2); delay(SPIN_SPEED);
    moveEye(-2,-1); delay(SPIN_SPEED);
    moveEye(-2,0);  delay(SPIN_SPEED);
    moveEye(-2,1);  delay(SPIN_SPEED);
    moveEye(-1,2);  delay(SPIN_SPEED);
    moveEye(0,2);   delay(SPIN_SPEED);
    moveEye(1,2);   delay(SPIN_SPEED);
    moveEye(2,1);   delay(SPIN_SPEED);
    moveEye(2,0);   delay(SPIN_SPEED);
  }
}

// Spin eye horizontally [BLOCKING]
void horizontalSpin() {
 
  // Force eye open
  while(blinkState > 0) {
    blinkState--;
    updateEyeAnim();
    delay(BLINK_SPEED);
  }
  
  // Position the pupil
  targetPupilX = 0;
  targetPupilY = 0;
  
  while(currentPupilX != targetPupilX || currentPupilY != targetPupilY) {
     updatePupilAnim();
     delay(PUPIL_SPEED);
  }
  
  delay(200);
  
  // Perform the spin
  for (int t=0; t<SPIN_COUNT; t++)
  {
    while(currentPupilX > -4){
      currentPupilX--;
      drawEye();
      delay(SPIN_SPEED);
    }
    
    currentPupilX = 4;
    
    while(currentPupilX > 0){
      currentPupilX--;
      drawEye();
      delay(SPIN_SPEED);
    }
  }
}

// Redraw blinking eye
void updateEyeAnim() {
  for(int b=0; b<8; b++){
    if(b <= blinkState || b >= 7-blinkState){
      pet.drawRow(b, B00000000); 
    } else {
      drawEyeRow(b, eye[b]); 
    }
  }
}

// Calculate and apply next anim step
void updatePupilAnim() {
  
  // Calculate number of steps needed
  int stepsX = abs(currentPupilX - targetPupilX);
  int stepsY = abs(currentPupilY - targetPupilY);

  // Make sure we have at least 1 step
  if ((stepsX == 0) && (stepsY == 0))
    return;
    
  // Calculate max steps
  int dirX = (targetPupilX >= currentPupilX) ? 1 : -1;
  int dirY = (targetPupilY >= currentPupilY) ? 1 : -1;
  int steps = (stepsX > stepsY) ? stepsX : stepsY;

  // Calculate next step
  float changeX = (float)stepsX / (float)steps;
  float changeY = (float)stepsY / (float)steps;
  
  // Apply next step
  int newX = limitPupilOffset(currentPupilX + round(changeX * dirX));
  int newY = limitPupilOffset(currentPupilY + round(changeY * dirY));
  
  // Redraw eye
  moveEye(newX, newY);
  
}

// Move the eye to a specific offset
void moveEye(int xOffset, int yOffset) {
  currentPupilX = xOffset;
  currentPupilY = yOffset;
  drawEye();
}

// Redraw the current eye
void drawEye() {
  for(int r=0; r<8; r++) {
    drawEyeRow(r, eye[r]);
  }
}

// Draw eye row merging in the current pupil
void drawEyeRow(int r, byte value) {
  
  // Calculate indexes for pupil rows (perform offset Y)
  int row1 = 3 - currentPupilY;
  int row2 = 4 - currentPupilY;
  
  if(r == row1 || r == row2){
    if(currentPupilX >= 0){
      pet.drawRow(r, value ^ (pupil >> currentPupilX));
    } else {
      pet.drawRow(r, value ^ (pupil << abs(currentPupilX)));
    }
  } else {
    pet.drawRow(r, value);
  }
}

// Ensure value falls within offset limits
int limitPupilOffset(int value) {
  if (value > PUPIL_MAX_OFFSET)
    return PUPIL_MAX_OFFSET;
  else if (value < PUPIL_MIN_OFFSET)
    return PUPIL_MIN_OFFSET;
  else
    return value;
}
