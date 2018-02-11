#ifndef SettingStates_h
#define SettingStates_h

#include "StateManager.h"

// =====================
// Setting States
// =====================

class SettingsOff : public State {
public:
  SettingsOff();
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class Channel : public State {
public:
  Channel();
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class Sensitivity : public State {
public:
  Sensitivity();
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class Brightness : public State {
public:
  Brightness();
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class Reset : public State {
public:
  Reset();
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

#endif
