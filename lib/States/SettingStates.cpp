#include "SettingStates.h"
#include "SystemStates.h"

SettingsOff::SettingsOff() {}

void SettingsOff::tap(StateManager *sm) {
  sm->setCurrent(new Channel());
  delete this;
}

void SettingsOff::press(StateManager *sm) {
  sm->setCurrent(new NormalOff());
  delete this;
}

Channel::Channel() {}

void Channel::tap(StateManager *sm) {
  sm->setCurrent(new Sensitivity());
  delete this;
}

void Channel::press(StateManager *sm) { sm->advanceChannel(); }

Sensitivity::Sensitivity() {}

void Sensitivity::tap(StateManager *sm) {
  sm->setCurrent(new Brightness());
  delete this;
}

void Sensitivity::press(StateManager *sm) { sm->advanceSensitivity(); }

Brightness::Brightness() {}

void Brightness::tap(StateManager *sm) {
  sm->setCurrent(new Reset());
  delete this;
}

void Brightness::press(StateManager *sm) { sm->advanceBrightness(); }

Reset::Reset() {}

void Reset::tap(StateManager *sm) {
  sm->setCurrent(new SettingsOff());
  delete this;
}

void Reset::press(StateManager *sm) { sm->resetSettings(); }