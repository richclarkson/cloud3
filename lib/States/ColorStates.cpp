#include "ColorStates.h"

// constructors
Neon::Neon() : State() {}

White::White() : State() {}

Ombre::Ombre() : State() {}

Fire::Fire() : State() {}

// press
void Neon::press(StateManager *sm) {
  delete this;
  sm->setColor(new White);
}

void White::press(StateManager *sm) {
  delete this;
  sm->setColor(new Ombre);
}

void Ombre::press(StateManager *sm) {
  delete this;
  sm->setColor(new Fire);
}

void Fire::press(StateManager *sm) {
  delete this;
  sm->setColor(new White);
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
