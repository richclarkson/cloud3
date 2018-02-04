#include "ColorStates.h"

Neon::Neon() {}

void Neon::press(StateManager *sm) {
  sm->setColor(new White());
  delete this;
}

White::White() {}

void White::press(StateManager *sm) {
  sm->setColor(new Ombre());
  delete this;
}

Ombre::Ombre() {}

void Ombre::press(StateManager *sm) {
  sm->setColor(new Fire());
  delete this;
}

Fire::Fire() {}

void Fire::press(StateManager *sm) {
  sm->setColor(new White());
  delete this;
}
