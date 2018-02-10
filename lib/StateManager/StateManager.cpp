#include "StateManager.h"
#include "States.h"

#include <iostream>
using namespace std;

StateManager::StateManager() {}

StateManager::StateManager(State *starting, State *startingColor) {
  setCurrent(starting);
  setColor(startingColor);
  channel = 8;
  sensitivity = 4;
  brightness = 4;
}

void StateManager::setCurrent(State *s) { this->current = s; }

int StateManager::getCurrentID() { return this->current->getID(); }

void StateManager::setColor(State *s) { this->colorSetting = s; }

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

void StateManager::registerFFT(void (*cb)(), float *fftArrayPointer) {
  this->fftCallBack = cb;
  this->fftArrayPtr = fftArrayPointer;
}

void StateManager::callFFTCallback() { this->fftCallBack(); }

void StateManager::registerLevel(void (*cb)(), float *levelValPointer) {
  this->levelCallback = cb;
  this->levelValPtr = levelValPointer;
}

void StateManager::callLevelCallback() { this->levelCallback(); }

void StateManager::update() { current->update(this->display); }

void StateManager::registerDisplayController(DisplayController *dc) {
  display = dc;
}

int StateManager::getDisplayTest() {
  return display->getTestVal();
}
