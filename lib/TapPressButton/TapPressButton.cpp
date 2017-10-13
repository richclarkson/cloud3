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

bool TapPressButton::isPress(){
  return pressTime > PRESS_THRESHOLD;
}

}

void TapPressButton::resetTimeStamp() { prevTimeStamp = 0; }

void TapPressButton::resetPressTime() { pressTime = 0; }
