#include "ColorStates.h"

// constructors
Neon::Neon() : State() {}
Neon::Neon(StateManager *sm) : State(sm) {}

White::White() : State() {}
White::White(StateManager *sm) : State(sm) {}

Ombre::Ombre() : State() {}
Ombre::Ombre(StateManager *sm) : State(sm) {}

Fire::Fire() {}
Fire::Fire(StateManager *sm) : State(sm) {}

// press
void Neon::press() {
  this->gsm->setColor(new White(this->gsm));
  delete this;
}

void White::press() {
  this->gsm->setColor(new Ombre(this->gsm));
  delete this;
}

void Ombre::press() {
  this->gsm->setColor(new Fire(this->gsm));
  delete this;
}

void Fire::press() {
  this->gsm->setColor(new White(this->gsm));
  delete this;
}

// update
void Neon::update(StateManager *sm) {
  sm->getDisplayController()->displayNeon();
}

void White::update(StateManager *sm) {
  sm->getDisplayController()->displayWhite();
}

void Ombre::update(StateManager *sm) {
  sm->getDisplayController()->displayOmbre();
}

void Fire::update(StateManager *sm) {
  sm->getDisplayController()->displayFire();
}
