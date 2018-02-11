#include "SystemStates.h"
#include "SettingStates.h"

// constructors
// previews should get called here
NormalOff::NormalOff() {}
NormalOff::NormalOff(StateManager *sm) : State(sm) {}

FallingDot::FallingDot() {}
FallingDot::FallingDot(StateManager *sm) : State(sm) {}

MiddleOut::MiddleOut() {}
MiddleOut::MiddleOut(StateManager *sm) : State(sm) {}

Ripple::Ripple() {}
Ripple::Ripple(StateManager *sm) : State(sm) {}

BangAndFade::BangAndFade() {}
BangAndFade::BangAndFade(StateManager *sm) : State(sm) {}

Rainbow::Rainbow() {}
Rainbow::Rainbow(StateManager *sm) : State(sm) {}

LampMode::LampMode() {}
LampMode::LampMode(StateManager *sm) : State(sm) {}
// ===============================

// tap methods
void NormalOff::tap() {
  FallingDot *fd = new FallingDot;
  fd->registerStateManager(gsm);
  gsm->setCurrent(fd);
  delete this;
  // gsm->setCurrent(new FallingDot(gsm));
}

void FallingDot::tap() {
  delete this;
  gsm->setCurrent(new MiddleOut(gsm));
}

void MiddleOut::tap() {
  delete this;
  gsm->setCurrent(new Ripple(gsm));
}

void Ripple::tap() {
  delete this;
  gsm->setCurrent(new BangAndFade(gsm));
}

void BangAndFade::tap() {
  delete this;
  gsm->setCurrent(new Rainbow(gsm));
}

void Rainbow::tap() {
  delete this;
  gsm->setCurrent(new LampMode(gsm));
}

void LampMode::tap() {
  delete this;
  gsm->setCurrent(new NormalOff(gsm));
}
// ==============================

// press methods
void NormalOff::press() {
  delete this;
  gsm->setCurrent(new SettingsOff(gsm));
}

void LampMode::press() { gsm->advanceColor(); }
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
