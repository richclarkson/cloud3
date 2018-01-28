#include "StateManager.h"

#if defined UNIT_TEST
#include <iostream>
using namespace std;
#define PRINT_MSG(msg) cout << msg << "\n"
#elif defined ARDUINO && STATE_MSG
#include <Arduino.h>
#define PRINT_MSG(msg) Serial.println(msg);
#else
#define PRINT_MSG(msg)
#endif

NormalOff::NormalOff() { PRINT_MSG("Normal Mode entered"); }

void NormalOff::tap(StateManager *sm) {
  sm->setCurrent(new FallingDot());
  delete this;
}

void NormalOff::press(StateManager *sm) {
  sm->setCurrent(new SettingsOff());
  delete this;
}

FallingDot::FallingDot() { PRINT_MSG("Falling Dot entered"); }

void FallingDot::tap(StateManager *sm) {
  sm->setCurrent(new MiddleOut());
  delete this;
}

void FallingDot::update(StateManager *sm) { sm->callLevelCallback(); }

MiddleOut::MiddleOut() { PRINT_MSG("Middle Out entered"); }

void MiddleOut::tap(StateManager *sm) {
  sm->setCurrent(new Ripple());
  delete this;
}

void MiddleOut::update(StateManager *sm) { sm->callLevelCallback(); }

Ripple::Ripple() { PRINT_MSG("Ripple entered"); }

void Ripple::tap(StateManager *sm) {
  sm->setCurrent(new BangAndFade());
  delete this;
}

void Ripple::update(StateManager *sm) { sm->callFFTCallback(); }

BangAndFade::BangAndFade() { PRINT_MSG("Bang and Fade entered"); }

void BangAndFade::tap(StateManager *sm) {
  sm->setCurrent(new Rainbow());
  delete this;
}

void BangAndFade::update(StateManager *sm) { sm->callLevelCallback(); }

Rainbow::Rainbow() { PRINT_MSG("Rainbow entered"); }

void Rainbow::tap(StateManager *sm) {
  sm->setCurrent(new LampMode());
  delete this;
}

void Rainbow::update(StateManager *sm) { sm->callLevelCallback(); }

LampMode::LampMode() { PRINT_MSG("Lamp Mode entered"); }

void LampMode::tap(StateManager *sm) {
  sm->setCurrent(new NormalOff());
  delete this;
}

void LampMode::press(StateManager *sm) { sm->advanceColor(); }

Neon::Neon() { PRINT_MSG("Neon entered"); }

void Neon::press(StateManager *sm) {
  sm->setColor(new White());
  delete this;
}

White::White() { PRINT_MSG("White entered"); }

void White::press(StateManager *sm) {
  sm->setColor(new Ombre());
  delete this;
}

Ombre::Ombre() { PRINT_MSG("Ombre entered"); }

void Ombre::press(StateManager *sm) {
  sm->setColor(new Fire());
  delete this;
}

Fire::Fire() { PRINT_MSG("Fire entered"); }

void Fire::press(StateManager *sm) {
  sm->setColor(new White());
  delete this;
}

SettingsOff::SettingsOff() { PRINT_MSG("Settings Mode entered"); }

void SettingsOff::tap(StateManager *sm) {
  sm->setCurrent(new Channel());
  delete this;
}

void SettingsOff::press(StateManager *sm) {
  sm->setCurrent(new NormalOff());
  delete this;
}

Channel::Channel() { PRINT_MSG("Channel Setting entered"); }

void Channel::tap(StateManager *sm) {
  sm->setCurrent(new Sensitivity());
  delete this;
}

void Channel::press(StateManager *sm) {
  sm->advanceChannel();
  PRINT_MSG(sm->getChannel());
}

Sensitivity::Sensitivity() { PRINT_MSG("Sensitivity Setting entered"); }

void Sensitivity::tap(StateManager *sm) {
  sm->setCurrent(new Brightness());
  delete this;
}

void Sensitivity::press(StateManager *sm) {
  sm->advanceSensitivity();
  PRINT_MSG(sm->getSensitivity());
}

Brightness::Brightness() { PRINT_MSG("Brightness Setting entered"); }

void Brightness::tap(StateManager *sm) {
  sm->setCurrent(new Reset());
  delete this;
}

void Brightness::press(StateManager *sm) {
  sm->advanceBrightness();
  PRINT_MSG(sm->getBrightness());
}

Reset::Reset() { PRINT_MSG("Reset Setting entered"); }

void Reset::tap(StateManager *sm) {
  sm->setCurrent(new SettingsOff());
  delete this;
}

void Reset::press(StateManager *sm) {
  sm->resetSettings();
  PRINT_MSG("Settings Reset");
}

StateManager::StateManager() {
  PRINT_MSG("State Manager intitialized");
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

void StateManager::registerFFTCallback(void (*cb)(), float* fftArrayPointer) {
  this->fftCallBack = cb;
  this->fftArrayPtr = fftArrayPointer;
}

void StateManager::callFFTCallback() { this->fftCallBack(); }

void StateManager::registerLevelCallback(void (*cb)(), float* levelValPointer) {
  this->levelCallback = cb;
  this->levelValPtr = levelValPointer;
}

void StateManager::callLevelCallback() { this->levelCallback(); }

void StateManager::update() { current->update(this); }
