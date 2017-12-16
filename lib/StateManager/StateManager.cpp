#include "StateManager.h"

#include <iostream>
using namespace std;

void NormalOff::tap(StateManager *sm) {
    sm->setCurrent(new FallingDot());
    cout << "   Falling Dot\n";
    delete this;
}

void NormalOff::press(StateManager *sm) {
    sm->setCurrent(new SettingsOff());
    cout << "   Settings Off\n";
    delete this;
}

void FallingDot::tap(StateManager *sm) {
    sm->setCurrent(new NormalOff());
    cout << "   Normal Off\n";
    delete this;
}

void SettingsOff::press(StateManager *sm) {
    sm->setCurrent(new NormalOff());
    cout << "   Normal Off\n";
    delete this;
}

StateManager::StateManager() {
    current = new NormalOff();
    cout << "   Init, Settings\n";
}

void StateManager::tap() {
    current->tap(this);
}

void StateManager::press() {
    current->press(this);
}