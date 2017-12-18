#include "StateManager.h"

#include <iostream>
using namespace std;

void NormalOff::tap(StateManager *sm) {
    sm->setCurrent(new FallingDot());
    delete this;
}

void NormalOff::press(StateManager *sm) {
    sm->setCurrent(new SettingsOff());
    delete this;
}

void FallingDot::tap(StateManager *sm) {
    sm->setCurrent(new MiddleOut());
    delete this;
}

void MiddleOut::tap(StateManager *sm) {
    sm->setCurrent(new Ripple());
    delete this;
}

void Ripple::tap(StateManager *sm) {
    sm->setCurrent(new BangAndFade());
    delete this;
}

void BangAndFade::tap(StateManager *sm) {
    sm->setCurrent(new Rainbow());
    delete this;
}

void Rainbow::tap(StateManager *sm) {
    sm->setCurrent(new LampMode());
    delete this;
}

void LampMode::tap(StateManager *sm) {
    sm->setCurrent(new NormalOff());
    delete this;
}

void LampMode::press(StateManager *sm) {
    sm->advanceColor();
}

void Neon::press(StateManager *sm) {
    sm->setColor(new White());
    delete this;
}

void White::press(StateManager *sm) {
    sm->setColor(new Neon());
    delete this;
}

void SettingsOff::press(StateManager *sm) {
    sm->setCurrent(new NormalOff());
    delete this;
}

StateManager::StateManager() {
    current = new NormalOff();
    colorSetting = new Neon();
}

void StateManager::tap() {
    current->tap(this);
}

void StateManager::press() {
    current->press(this);
}

void StateManager::advanceColor() {
    colorSetting->press(this);
}