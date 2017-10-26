#include "TapPressButton.h"

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
