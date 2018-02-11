#include "StateManager.h"
#include "States.h"

//=== constructors ==========
StateManager::StateManager() {}

StateManager::StateManager(State *starting, State *startingColor) {
  setCurrent(starting);
  setColor(startingColor);
  current->registerStateManager(this);
  colorSetting->registerStateManager(this);
  channel = 8;
  sensitivity = 4;
  brightness = 4;
}
//============================

//=== setters ================
void StateManager::setCurrent(State *s) { this->current = s; }

void StateManager::setColor(State *s) { this->colorSetting = s; }
//============================

//=== getters ================
State *StateManager::getCurrent() { return current; }

State *StateManager::getColor() { return colorSetting; }

DisplayController *StateManager::getDisplayController() { return display; }

int StateManager::getChannel() { return channel; }

int StateManager::getSensitivity() { return sensitivity; }

int StateManager::getBrightness() { return brightness; }
//============================

//=== dependency injection ===
void StateManager::registerDisplayController(DisplayController *dc) {
  display = dc;
}

void StateManager::registerFFT(void (*cb)(), float *fftArrayPointer) {
  this->fftCallBack = cb;
  this->fftArrayPtr = fftArrayPointer;
}

void StateManager::registerLevel(void (*cb)(), float *levelValPointer) {
  this->levelCallback = cb;
  this->levelValPtr = levelValPointer;
}
//============================

//=== operations =============

void StateManager::tap() { current->tap(this); }

void StateManager::press() { current->press(this); }

void StateManager::update() { current->update(this); }

void StateManager::advanceColor() { colorSetting->press(); }

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

void StateManager::updateFFT() { this->fftCallBack(); }

void StateManager::updateLevel() { this->levelCallback(); }
//============================
