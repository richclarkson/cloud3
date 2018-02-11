#include "SaberDisplayController.h"
#include "FastLED.h"

SaberDisplayController::SaberDisplayController() {
  whiteFlag = false;
}

void SaberDisplayController::turnOff() {
  // include the code to turn off the LEDs here
}

void SaberDisplayController::displayFallingDot() {
  // add state display code here
}

void SaberDisplayController::displayMiddleOut() {
  // add state display code here
}

void SaberDisplayController::displayRipple() {
  // add state display code here
}

void SaberDisplayController::displayBangAndFade() {
  // add state display code here
}

void SaberDisplayController::displayRainbow() {
  // add state display code here
}

// Lamp Mode Displays
void SaberDisplayController::displayNeon() {
  whiteFlag = false;
  // add lamp display code here
}

void SaberDisplayController::displayWhite() {
  if (!whiteFlag) {
    // add lamp display code here
    whiteFlag = true;
  }
}

void SaberDisplayController::displayOmbre() {
  whiteFlag = false;
  // add lamp display code here
}

void SaberDisplayController::displayFire() {
  whiteFlag = false;
  // add lamp display code here
}
