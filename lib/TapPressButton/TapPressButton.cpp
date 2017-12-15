#include "TapPressButton.h"

TapPressButton::TapPressButton() {
  TAP_THRESHOLD = 50;
  TAP_LENGTH = 300;
  PRESS_THRESHOLD = 500;
  PRESS_LENGTH = 500;
  tapTimer = 0;
  pressTimer = 0;
  prevTimerVal = 0;
  currentButtonState = false;
  prevButtonState = false;
  pressType = 0;
}

void TapPressButton::update(bool input, unsigned long timerVal) {
  setStates(input);
  setTimers(timerVal);
  setPressType();
  if (stateHasChanged() || currentButtonState == false) {
    pressTimer = 0;
    tapTimer = 0;
  }
}

void TapPressButton::setTimers(unsigned long timerVal) {
  setTapTimer(timerVal);
  setPressTimer(timerVal);
  prevTimerVal = timerVal;
}

void TapPressButton::setTapTimer(unsigned long timerVal) {
  tapTimer += timerVal - prevTimerVal;
}

void TapPressButton::setPressTimer(unsigned long timerVal) {
  pressTimer += timerVal - prevTimerVal;
}

void TapPressButton::setStates(bool buttonInput) {
  prevButtonState = currentButtonState;
  currentButtonState = buttonInput;
}

bool TapPressButton::pressTypeIsTap() {
  return 
    stateHasChanged() && 
    currentButtonState == false &&
    isPressInTapWindow();
}

bool TapPressButton::pressTypeIsPress() {
  return currentButtonState == true && isPressInPressWindow();
}

void TapPressButton::setPressType() {
  if (pressTypeIsTap())
    pressType = 1; // pressType 1 = Tap
  else if (pressTypeIsPress())
    pressType = 2; // pressType 2 = Press
  else
    pressType = 0;
}

bool TapPressButton::stateHasChanged() {
  return currentButtonState != prevButtonState;
}

bool TapPressButton::isPressInTapWindow() {
  return tapTimer >= TAP_THRESHOLD && tapTimer < TAP_THRESHOLD + TAP_LENGTH;
}

bool TapPressButton::isPressInPressWindow() {
  return pressTimer >= PRESS_THRESHOLD;
}

bool TapPressButton::isTap() {
  if (pressType == 1) {
    pressType = 0;
    tapTimer = 0;
    return true;
  }
  return false;
}

bool TapPressButton::isPress() {
  if (pressType == 2) {
    pressType = 0;
    pressTimer = 0;
    return true;
  }
  return false;
}
