#ifndef SettingStates_h
#define SettingStates_h

#include "StateManager.h"

// =====================
// Setting States
// =====================

class SettingsOff : public State {
public:
  SettingsOff();
  ~SettingsOff();
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class Channel : public State {
public:
  Channel();
  ~Channel();
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class Sensitivity : public State {
public:
  Sensitivity();
  ~Sensitivity();
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class Brightness : public State {
public:
  Brightness();
  ~Brightness();
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class Reset : public State {
public:
  Reset();
  ~Reset();
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

#endif
