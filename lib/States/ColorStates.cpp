#include "ColorStates.h"

// constructors
Neon::Neon() : State() {}
Neon::~Neon() {}

White::White() : State() {}
White::~White() {}

Ombre::Ombre() : State() {}
Ombre::~Ombre() {}

Fire::Fire() : State() {}
Fire::~Fire() {}

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
