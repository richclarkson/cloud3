#include "StateManager.h"

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

void SettingsOff::tap(StateManager *sm) {
    sm->setCurrent(new Channel());
    delete this;
}

void SettingsOff::press(StateManager *sm) {
    sm->setCurrent(new NormalOff());
    delete this;
}

void Channel::tap(StateManager *sm) {
    sm->setCurrent(new Sensitivity());
    delete this;
}

void Channel::press(StateManager *sm) {
    sm->advanceChannel();
}

void Sensitivity::tap(StateManager *sm) {
    sm->setCurrent(new Brightness());
    delete this;
}

void Sensitivity::press(StateManager *sm) {
    sm->advanceSensitivity();
}

void Brightness::tap(StateManager *sm) {
    sm->setCurrent(new Reset());
    delete this;
}

void Brightness::press(StateManager *sm) {
    sm->advanceBrightness();
}

void Reset::tap(StateManager *sm) {
    sm->setCurrent(new SettingsOff());
    delete this;
}

void Reset::press(StateManager *sm) {
    sm->resetSettings();
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

void StateManager::advanceChannel() {
    this->channel ++;
    if (this->channel > 8) {
        this->channel = 0;
    }
    //TODO: save this value to EEPROM
}

void StateManager::advanceSensitivity() {
    this->sensitvity ++;
    if (this->sensitvity >= 8) {
        this->sensitvity = 0;
    }
    //TODO: save this value to EEPROM
}

void StateManager::advanceBrightness() {
    this->brightness ++;
    if (this->brightness >= 8) {
        this->brightness = 0;
    }
    //TODO: save this value to EEPROM
}

void StateManager::resetSettings() {
    this->channel = 0;
    this->sensitvity = 0;
    this->brightness = 0;
    //TODO: Saves values to EEPROM
}
