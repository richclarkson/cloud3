#ifndef StateManager_h
#define StateManager_h

#include <iostream>

class StateManager {
  class State *current;

public:
  StateManager();
  void setCurrent(State *s) { current = s; }
  void tap();
  void press();
};

class State {
public:
  virtual void tap(StateManager *sm) {}
  virtual void press(StateManager *sm) {}
};

class NormalOff : public State {
public:
  NormalOff(){};
  ~NormalOff(){};
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class FallingDot : public State {
public:
  FallingDot(){};
  ~FallingDot(){};
  void tap(StateManager *sm);
};

class SettingsOff : public State {
public:
  SettingsOff(){};
  ~SettingsOff(){};
  void press(StateManager *sm);
};

#endif
