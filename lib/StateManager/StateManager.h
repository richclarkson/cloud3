#ifndef StateManager_h
#define StateManager_h

#include <iostream>
using namespace std;

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
// ===========================
// Declare system states here
// ===========================

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

class MiddleOut : public State {
public:
  MiddleOut(){};
  ~MiddleOut(){};
  void tap(StateManager *sm);
};

class Ripple : public State {
public:
  Ripple(){};
  ~Ripple(){};
  void tap(StateManager *sm);
};

class BangAndFade : public State {
public:
  BangAndFade(){};
  ~BangAndFade(){};
  void tap(StateManager *sm);
};

class Rainbow : public State {
public:
  Rainbow(){};
  ~Rainbow(){};
  void tap(StateManager *sm);
};

class LampMode : public State {
public:
  LampMode(){};
  ~LampMode(){};
  void tap(StateManager *sm);
  StateManager lampState
};

class SettingsOff : public State {
public:
  SettingsOff(){};
  ~SettingsOff(){};
  void press(StateManager *sm);
};

#endif
