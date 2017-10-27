#include "TapPressButton.h"

TapPressButton::TapPressButton() {
  pressTime = 0;
  tapThreshold = 50;
  tapLength = 300;
  pressThreshold = 500;
  currentButtonState = false;
  prevButtonState = false;
  pressType = 0;
}

void TapPressButton::updateInput(bool input, unsigned long timerVal) {
  setStates(input);
  pressTime += timerVal;
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
  } else {
    pressType = 0;
  }
}

bool TapPressButton::stateHasChanged() {
  return currentButtonState != prevButtonState;
}

bool TapPressButton::getCurrentState() { return currentButtonState; }

unsigned long TapPressButton::getPressTime() { return pressTime; }

bool TapPressButton::isPressInTapWindow() {
  return pressTime > tapThreshold && pressTime < tapThreshold + tapLength;
}

bool TapPressButton::isPressInPressWindow() {
  return pressTime > pressThreshold;
}

bool TapPressButton::isTap() { return pressType == 1; }
