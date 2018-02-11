#include "ColorStates.h"

Neon::Neon() {}
White::White() {}
Ombre::Ombre() {}
Fire::Fire() {}

void Neon::press(StateManager *sm) {
  delete this;
  sm->setColor(new White());
}

void White::press(StateManager *sm) {
  delete this;
  sm->setColor(new Ombre());
}

void Ombre::press(StateManager *sm) {
  delete this;
  sm->setColor(new Fire());
}

void Fire::press(StateManager *sm) {
  delete this;
  sm->setColor(new White());
}

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


