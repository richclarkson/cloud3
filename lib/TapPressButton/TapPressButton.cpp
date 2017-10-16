#include "TapPressButton.h"

TapPressButton::TapPressButton() {
  prevTimeStamp = 0;
  pressTime = 0;
  pressCount = 0;
  buttonState = false;

  // set default values for button behavior
  setDebounce(50);
  setTapLength(300);
  setPressThreshold(500);
  setPressLength(1000);
}

void TapPressButton::update(bool btnVal, unsigned long timerVal) {
  // This function should be called repeatedly in the main loop.
  buttonState = btnVal;
  if (buttonState) {
    updatePressTime(timerVal);
  } else {
    resetPressTime();
  }
}

bool TapPressButton::isDebounced() {
  return pressTime > DEBOUNCE_THRESHOLD;
}

bool TapPressButton::isTap() {
  return !buttonState && pressTime > DEBOUNCE_THRESHOLD &&
         pressTime <= DEBOUNCE_THRESHOLD + TAP_LENGTH;
}

bool TapPressButton::isPress() {
  return buttonState && pressTime > PRESS_THRESHOLD;
}

void TapPressButton::updatePressTime(unsigned long newTime) {
  if (newTime > prevTimeStamp) {
    pressTime += newTime - prevTimeStamp;
    prevTimeStamp = newTime;
  }
}

void TapPressButton::resetPressTime() { pressTime = 0; }

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

char TapPressButton::getPressType() { return pressType; }

void TapPressButton::setDebounce(unsigned long debounceTime) {
  DEBOUNCE_THRESHOLD = debounceTime;
}

void TapPressButton::setTapLength(unsigned long tapTime) {
  TAP_LENGTH = tapTime;
}
void TapPressButton::setPressThreshold(unsigned long pressThreshold) {
  PRESS_THRESHOLD = pressThreshold;
}

void TapPressButton::setPressLength(unsigned long pressLength) {
  PRESS_LENGTH = pressLength;
}
