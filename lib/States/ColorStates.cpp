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
