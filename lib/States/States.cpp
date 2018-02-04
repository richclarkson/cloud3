#include "States.h"

NormalOff::NormalOff() {}

void NormalOff::tap(StateManager *sm) {
  sm->setCurrent(new FallingDot());
  delete this;
}

void NormalOff::press(StateManager *sm) {
  sm->setCurrent(new SettingsOff());
  delete this;
}

FallingDot::FallingDot() {}

void FallingDot::tap(StateManager *sm) {
  sm->setCurrent(new MiddleOut());
  delete this;
}

void FallingDot::update(StateManager *sm) { sm->callLevelCallback(); }

MiddleOut::MiddleOut() {}

void MiddleOut::tap(StateManager *sm) {
  sm->setCurrent(new Ripple());
  delete this;
}

void MiddleOut::update(StateManager *sm) { sm->callLevelCallback(); }

Ripple::Ripple() {}

void Ripple::tap(StateManager *sm) {
  sm->setCurrent(new BangAndFade());
  delete this;
}

void Ripple::update(StateManager *sm) { sm->callFFTCallback(); }

BangAndFade::BangAndFade() {}

void BangAndFade::tap(StateManager *sm) {
  sm->setCurrent(new Rainbow());
  delete this;
}

void BangAndFade::update(StateManager *sm) { sm->callLevelCallback(); }

Rainbow::Rainbow() {}

void Rainbow::tap(StateManager *sm) {
  sm->setCurrent(new LampMode());
  delete this;
}

void Rainbow::update(StateManager *sm) { sm->callLevelCallback(); }

LampMode::LampMode() {}

void LampMode::tap(StateManager *sm) {
  sm->setCurrent(new NormalOff());
  delete this;
}

void LampMode::press(StateManager *sm) { sm->advanceColor(); }

Neon::Neon() {}

void Neon::press(StateManager *sm) {
  sm->setColor(new White());
  delete this;
}

White::White() {}

void White::press(StateManager *sm) {
  sm->setColor(new Ombre());
  delete this;
}

Ombre::Ombre() {}

void Ombre::press(StateManager *sm) {
  sm->setColor(new Fire());
  delete this;
}

Fire::Fire() {}

void Fire::press(StateManager *sm) {
  sm->setColor(new White());
  delete this;
}

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
