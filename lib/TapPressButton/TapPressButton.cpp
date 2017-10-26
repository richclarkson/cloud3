#include "TapPressButton.h"

TapPressButton::TapPressButton() {
  pressTime = 0;
  tapThreshold = 50;
  tapLength = 300;
  pressThreshold = 500;
  currentButtonState = false;
  prevButtonState = false;
}

void TapPressButton::updateInput(bool input, unsigned long timerVal) {
  prevButtonState = currentButtonState;
  currentButtonState = input;
  pressTime += timerVal;
}

bool TapPressButton::getCurrentState() { return currentButtonState; }

bool TapPressButton::hasStateChanged() {
  return currentButtonState != prevButtonState;
}

unsigned long TapPressButton::getPressTime() { return pressTime; }

bool TapPressButton::isPressInTapWindow() {
  return pressTime > tapThreshold && pressTime < tapThreshold + tapLength;
}

bool TapPressButton::isPressInPressWindow() {
  return pressTime > pressThreshold;
}