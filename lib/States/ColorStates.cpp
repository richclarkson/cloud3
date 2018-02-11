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
