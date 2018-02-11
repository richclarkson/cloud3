#include "SettingStates.h"
#include "SystemStates.h"

// constructors
SettingsOff::SettingsOff() : State() {}

Channel::Channel() : State() {}

Sensitivity::Sensitivity() : State() {}

Brightness::Brightness() : State() {}

Reset::Reset() : State() {}

// tap methods
void SettingsOff::tap(StateManager *sm) {
  delete this;
  sm->setCurrent(new Channel);
}

void Channel::tap(StateManager *sm) {
  delete this;
  sm->setCurrent(new Sensitivity);
}

void Sensitivity::tap(StateManager *sm) {
  delete this;
  sm->setCurrent(new Brightness);
}

void Brightness::tap(StateManager *sm) {
  delete this;
  sm->setCurrent(new Reset);
}

void Reset::tap(StateManager *sm) {
  delete this;
  sm->setCurrent(new SettingsOff);
}

// press methods
void SettingsOff::press(StateManager *sm) {
  delete this;
  sm->setCurrent(new NormalOff);
}

void Channel::press(StateManager *sm) { sm->advanceChannel(); }

void Sensitivity::press(StateManager *sm) { sm->advanceSensitivity(); }

void Brightness::press(StateManager *sm) { sm->advanceBrightness(); }

void Reset::press(StateManager *sm) { sm->resetSettings(); }
