#include "TapPressButton.h"

TapPressButton::TapPressButton() {
  prevTimeStamp = 0;
  pressTime = 0;

  // set default values for button behavior
  DEBOUNCE_THRESHOLD = 50;
  TAP_LENGTH = 300;
  PRESS_THRESHOLD = 500;
  PRESS_LENGTH = 1000;
}

void TapPressButton::update(bool btnVal, unsigned long timerVal) {
  // This function should be called repeatedly in the main loop.
  if (btnVal) {
    updatePressTime(timerVal);
  } else {
    if (isTap()) {
      pressType = 1;
    }
    resetPressTime();
    resetTimeStamp();
  }
}

void TapPressButton::updatePressTime(unsigned long newTime) {
  if (newTime > prevTimeStamp) {
    pressTime += newTime - prevTimeStamp;
  }
}

bool TapPressButton::isTap() {
  return pressTime > DEBOUNCE_THRESHOLD &&
         pressTime <= DEBOUNCE_THRESHOLD + TAP_LENGTH;
}

bool TapPressButton::isPress() { return pressTime > PRESS_THRESHOLD; }

int TapPressButton::getPressCount() {
  int pressCount = 0;
  int pressLoopCounter = 1;
  if (isPress()) {
    while (pressCount == 0) {
      if (pressTime - PRESS_THRESHOLD <= PRESS_LENGTH * pressLoopCounter) {
        pressLoopCounter++;
      } else {
        pressCount = pressLoopCounter;
      }
      // set a max to avoid in infinite loop
      if (pressLoopCounter > 50) {
        pressCount = -1;
      }
    }
  }
  return pressCount;
}

void TapPressButton::resetTimeStamp() { prevTimeStamp = 0; }

void TapPressButton::resetPressTime() { pressTime = 0; }
