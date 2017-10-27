#include "TapPressButton.h"

TapPressButton::TapPressButton() {
  pressTime = 0;
  prevTimerVal = 0;
  tapThreshold = 50;
  tapLength = 300;
  pressThreshold = 500;
  currentButtonState = false;
  prevButtonState = false;
  pressType = 0;
}

void TapPressButton::updateInput(bool input, unsigned long timerVal) {
  setStates(input);
  pressTime += timerVal - prevTimerVal;
  prevTimerVal = timerVal;
  setPressType();
  if (stateHasChanged() || currentButtonState == false) {
    pressTime = 0;
  } else {
  }
}

void TapPressButton::setStates(bool btnInput) {
  prevButtonState = currentButtonState;
  currentButtonState = btnInput;
}

void TapPressButton::setPressType() {
  if (stateHasChanged() && !currentButtonState && isPressInTapWindow()) {
    pressType = 1; // pressType 1 = Tap
  } else if (currentButtonState && isPressInPressWindow()) {
    pressType = 2; // pressType 2 = Press
  } else {
    pressType = 0;
  }
}

bool TapPressButton::stateHasChanged() {
  return currentButtonState != prevButtonState;
}

bool TapPressButton::isPressInTapWindow() {
  return pressTime > tapThreshold && pressTime < tapThreshold + tapLength;
}

bool TapPressButton::isPressInPressWindow() {
  return pressTime > pressThreshold;
}

bool TapPressButton::isTap() { return pressType == 1; }

bool TapPressButton::isPress() { return pressType == 2; }
