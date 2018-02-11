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
void Neon::update() { this->gsm->getDisplayController()->displayNeon(); }

void White::update() { this->gsm->getDisplayController()->displayWhite(); }

void Ombre::update() { this->gsm->getDisplayController()->displayOmbre(); }

void Fire::update() { this->gsm->getDisplayController()->displayFire(); }
