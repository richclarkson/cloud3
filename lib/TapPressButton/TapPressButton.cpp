#include "TapPressButton.h"

TapPressButton::TapPressButton() {
  prevTimeStamp = 0;
  pressTime = 0;
  pressCount = 0;
  pressType = 0;
  prevButtonState = false;

  // set default values for button behavior
  setDebounce(50);
  setTapLength(300);
  setPressThreshold(500);
  setPressLength(1000);
}

void TapPressButton::update(bool btnVal, unsigned long timerVal) {
  // This function should be called repeatedly in the main loop.
  pressType = 0;
  if (btnVal) {
    if (!prevButtonState) {
      prevTimeStamp = timerVal;
      prevButtonState = true;
    }
    updatePressTime(timerVal);
    if (isPressInPressWindow()) {
      pressType = 2;
    }
  } else {
    if (isPressInTapWindow()) {
      pressType = 1;
    }
    resetPressTime();
    prevTimeStamp = timerVal;
    prevButtonState = false;
  }
}

void TapPressButton::updatePressTime(unsigned long newTime) {
  // check for unlikely timer rollover
  if (newTime < prevTimeStamp) {
    prevTimeStamp = newTime;
  }
  pressTime += newTime - prevTimeStamp;
  prevTimeStamp = newTime;
}

void TapPressButton::resetPressTime() { pressTime = 0; }

bool TapPressButton::isPressInTapWindow() {
  return pressTime >= DEBOUNCE_THRESHOLD &&
         pressTime < DEBOUNCE_THRESHOLD + TAP_LENGTH;
}

bool TapPressButton::isPressInPressWindow() {
  return pressTime >= PRESS_THRESHOLD;
}

bool TapPressButton::isTap() { return pressType == 1; }

bool TapPressButton::isPress() { return pressType == 2; }

int TapPressButton::getPressCount() {
  pressCount = 0;
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
