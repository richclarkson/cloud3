#include "SystemStates.h"
#include "SettingStates.h"

// constructors
// previews should get called here
NormalOff::NormalOff() : State() {}
NormalOff::~NormalOff() {}

FallingDot::FallingDot() : State() {}
FallingDot::~FallingDot() {}

MiddleOut::MiddleOut() : State() {}
MiddleOut::~MiddleOut() {}

Ripple::Ripple() : State() {}
Ripple::~Ripple() {}

BangAndFade::BangAndFade() : State() {}
BangAndFade::~BangAndFade() {}

Rainbow::Rainbow() : State() {}
Rainbow::~Rainbow() {}

LampMode::LampMode() : State() {}
LampMode::~LampMode() {}
// ===============================

// tap methods
void NormalOff::tap(StateManager *sm) {
  delete this;
  sm->setCurrent(new FallingDot);
}

void FallingDot::tap(StateManager *sm) {
  delete this;
  sm->setCurrent(new MiddleOut);
}

void MiddleOut::tap(StateManager *sm) {
  delete this;
  sm->setCurrent(new Ripple);
}

void Ripple::tap(StateManager *sm) {
  delete this;
  sm->setCurrent(new BangAndFade);
}

void BangAndFade::tap(StateManager *sm) {
  delete this;
  sm->setCurrent(new Rainbow);
}

void Rainbow::tap(StateManager *sm) {
  delete this;
  sm->setCurrent(new LampMode);
}

void LampMode::tap(StateManager *sm) {
  delete this;
  sm->setCurrent(new NormalOff);
}
// ==============================

// press methods
void NormalOff::press(StateManager *sm) {
  delete this;
  sm->setCurrent(new SettingsOff);
}

void LampMode::press(StateManager *sm) { sm->advanceColor(); }
// ==============================

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

void LampMode::update(StateManager *sm) { sm->getColor()->update(sm); }
// ==============================
