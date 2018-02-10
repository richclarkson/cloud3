#include "SystemStates.h"
#include "SettingStates.h"

NormalOff::NormalOff() {}

void NormalOff::tap(StateManager *sm) {
  sm->setCurrent(new FallingDot());
  delete this;
}

void NormalOff::press(StateManager *sm) {
  sm->setCurrent(new SettingsOff());
  delete this;
}

void NormalOff::update(DisplayController *dc) { dc->turnOff(); }

FallingDot::FallingDot() {}

void FallingDot::tap(StateManager *sm) {
  sm->setCurrent(new MiddleOut());
  delete this;
}

void FallingDot::update(DisplayController *dc) { dc->fallingDot(); }

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
