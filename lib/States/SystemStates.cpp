#include "SystemStates.h"
#include "SettingStates.h"

// constructors
// previews should get called here
NormalOff::NormalOff() {}
FallingDot::FallingDot() {}
MiddleOut::MiddleOut() {}
Ripple::Ripple() {}
BangAndFade::BangAndFade() {}
Rainbow::Rainbow() {}
LampMode::LampMode() {}

// tap methods
void NormalOff::tap(StateManager *sm) {
  sm->setCurrent(new FallingDot());
  delete this;
}

void FallingDot::tap(StateManager *sm) {
  sm->setCurrent(new MiddleOut());
  delete this;
}

void MiddleOut::tap(StateManager *sm) {
  sm->setCurrent(new Ripple());
  delete this;
}

void Ripple::tap(StateManager *sm) {
  sm->setCurrent(new BangAndFade());
  delete this;
}

void BangAndFade::tap(StateManager *sm) {
  sm->setCurrent(new Rainbow());
  delete this;
}

void Rainbow::tap(StateManager *sm) {
  sm->setCurrent(new LampMode());
  delete this;
}

void LampMode::tap(StateManager *sm) {
  sm->setCurrent(new NormalOff());
  delete this;
}

// press methods
void NormalOff::press(StateManager *sm) {
  sm->setCurrent(new SettingsOff());
  delete this;
}

void LampMode::press(StateManager *sm) { sm->advanceColor(); }

// update methods
void NormalOff::update(StateManager *sm) {
  sm->getDisplayController()->turnOff();
}

void FallingDot::update(StateManager *sm) {
  sm->getDisplayController()->displayFallingDot();
}

void MiddleOut::update(StateManager *sm) {
  sm->getDisplayController()->displayMiddleOut();
}

void Ripple::update(StateManager *sm) {
  sm->getDisplayController()->displayRipple();
}

void BangAndFade::update(StateManager *sm) {
  sm->getDisplayController()->displayBangAndFade();
}

void Rainbow::update(StateManager *sm) {
  sm->getDisplayController()->displayRainbow();
}

void LampMode::update(StateManager *sm) {
  sm->getColor()->update(sm);
}
