#include "ColorStates.h"

Neon::Neon() {}
White::White() {}
Ombre::Ombre() {}
Fire::Fire() {}

void Neon::press(StateManager *sm) {
  sm->setColor(new White());
  delete this;
}

void White::press(StateManager *sm) {
  sm->setColor(new Ombre());
  delete this;
}

void Ombre::press(StateManager *sm) {
  sm->setColor(new Fire());
  delete this;
}

void Fire::press(StateManager *sm) {
  sm->setColor(new White());
  delete this;
}
