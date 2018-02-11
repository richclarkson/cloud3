#include "SettingStates.h"
#include "SystemStates.h"

// constructors
SettingsOff::SettingsOff() {}
SettingsOff::SettingsOff(StateManager *sm) : State(sm) {}

Channel::Channel() {}
Channel::Channel(StateManager *sm) : State(sm) {}

Sensitivity::Sensitivity() {}
Sensitivity::Sensitivity(StateManager *sm) : State(sm) {}

Brightness::Brightness() {}
Brightness::Brightness(StateManager *sm) : State(sm) {}

Reset::Reset() {}
Reset::Reset(StateManager *sm) : State(sm) {}

// tap methods
void SettingsOff::tap(StateManager *sm) {
  this->gsm->setCurrent(new Channel(this->gsm));
  delete this;
}

void Channel::tap(StateManager *sm) {
  this->gsm->setCurrent(new Sensitivity(this->gsm));
  delete this;
}

void Sensitivity::tap(StateManager *sm) {
  this->gsm->setCurrent(new Brightness(this->gsm));
  delete this;
}

void Brightness::tap(StateManager *sm) {
  this->gsm->setCurrent(new Reset(this->gsm));
  delete this;
}

void Reset::tap(StateManager *sm) {
  this->gsm->setCurrent(new SettingsOff(this->gsm));
  delete this;
}

// press methods
void SettingsOff::press() {
  this->gsm->setCurrent(new NormalOff(this->gsm));
  delete this;
}

void Channel::press() { this->gsm->advanceChannel(); }

void Sensitivity::press() { this->gsm->advanceSensitivity(); }

void Brightness::press() { this->gsm->advanceBrightness(); }

void Reset::press() { this->gsm->resetSettings(); }
