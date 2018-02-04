#include "StateManager.h"
#include "States.h"

StateManager::StateManager() {  
  current = new NormalOff();
  colorSetting = new Neon();
  channel = 8;
  sensitivity = 4;
  brightness = 4;
}

void StateManager::tap() { current->tap(this); }

void StateManager::press() { current->press(this); }

void StateManager::advanceColor() { colorSetting->press(this); }

void StateManager::advanceChannel() {
  this->channel++;
  if (this->channel > 8) {
    this->channel = 0;
  }
  // TODO: save this value to EEPROM
}

void StateManager::advanceSensitivity() {
  this->sensitivity++;
  if (this->sensitivity >= 8) {
    this->sensitivity = 0;
  }
  // TODO: save this value to EEPROM
}

void StateManager::advanceBrightness() {
  this->brightness++;
  if (this->brightness >= 8) {
    this->brightness = 0;
  }
  // TODO: save this value to EEPROM
}

void StateManager::resetSettings() {
  this->channel = 0;
  this->sensitivity = 0;
  this->brightness = 0;
  // TODO: Saves values to EEPROM
}

void StateManager::registerFFT(void (*cb)(), float* fftArrayPointer) {
  this->fftCallBack = cb;
  this->fftArrayPtr = fftArrayPointer;
}

void StateManager::callFFTCallback() { this->fftCallBack(); }

void StateManager::registerLevel(void (*cb)(), float* levelValPointer) {
  this->levelCallback = cb;
  this->levelValPtr = levelValPointer;
}

void StateManager::callLevelCallback() { this->levelCallback(); }

void StateManager::update() { current->update(this); }
