#include "TapPressButton.h"

TapPressButton::TapPressButton() {
  TAP_THRESHOLD = 50;
  TAP_LENGTH = 300;
  PRESS_THRESHOLD = 500;
  pressTime = 0;
  prevTimerVal = 0;
  currentButtonState = false;
  prevButtonState = false;
  pressType = 0;
}

void TapPressButton::update(bool input, unsigned long timerVal) {
  setStates(input);
  pressTime += timerVal - prevTimerVal;
  prevTimerVal = timerVal;
  setPressType();
  if (stateHasChanged() || currentButtonState == false) {
    pressTime = 0;
  } else {
  }
}

void TapPressButton::setStates(bool buttonInput) {
  prevButtonState = currentButtonState;
  currentButtonState = buttonInput;
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
  return pressTime >= TAP_THRESHOLD && pressTime < TAP_THRESHOLD + TAP_LENGTH;
}

bool TapPressButton::isPressInPressWindow() {
  return pressTime >= PRESS_THRESHOLD;
}

bool TapPressButton::isTap() { return pressType == 1; }

bool TapPressButton::isPress() { return pressType == 2; }
