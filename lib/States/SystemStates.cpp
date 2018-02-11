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
  this->gsm->setCurrent(new FallingDot(this->gsm));
  delete this;
}

void FallingDot::tap() {
  this->gsm->setCurrent(new MiddleOut(this->gsm));
  delete this;
}

void MiddleOut::tap() {
  this->gsm->setCurrent(new Ripple(this->gsm));
  delete this;
}

void Ripple::tap() {
  this->gsm->setCurrent(new BangAndFade(this->gsm));
  delete this;
}

void BangAndFade::tap() {
  this->gsm->setCurrent(new Rainbow(this->gsm));
  delete this;
}

void Rainbow::tap() {
  this->gsm->setCurrent(new LampMode(this->gsm));
  delete this;
}

void LampMode::tap() {
  this->gsm->setCurrent(new NormalOff(this->gsm));
  delete this;
}
// ==============================

// press methods
void NormalOff::press() {
  this->gsm->setCurrent(new SettingsOff(this->gsm));
  delete this;
}

void LampMode::press() { this->gsm->advanceColor(); }
// ==============================

// update methods
void NormalOff::update() {
  this->gsm->getDisplayController()->turnOff();
}

void FallingDot::update() {
  this->gsm->getDisplayController()->displayFallingDot();
}

void MiddleOut::update() {
  this->gsm->getDisplayController()->displayMiddleOut();
}

void Ripple::update() {
  this->gsm->getDisplayController()->displayRipple();
}

void BangAndFade::update() {
  this->gsm->getDisplayController()->displayBangAndFade();
}

void Rainbow::update() {
  this->gsm->getDisplayController()->displayRainbow();
}

void LampMode::update() {
  this->gsm->getColor()->update();
}
// ==============================
