#include "SaberStateHandler.h"

SaberStateHandler::SaberStateHandler(void) {
  mainState = BasicStateMachine(2);
  normalModes = BasicStateMachine(7);
  settingModes = BasicStateMachine(5);
}

int SaberStateHandler::getMainState() { return this->mainState.getState(); }

int SaberStateHandler::getNormalMode() { return this->normalModes.getState(); }

int SaberStateHandler::getSettingMode(void) {
  return this->settingModes.getState();
}

void SaberStateHandler::tap() {
  if (this->mainState.is(0)) {
    normalModes.next();
  } 
  else if (this->mainState.is(1)) {
    settingModes.next();
  }
}

void SaberStateHandler::press() {
  if (this->normalModes.is(0) && this->settingModes.is(0)) {
    mainState.next();
  }
}
