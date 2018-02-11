#ifndef SystemStates_h
#define SystemStates_h

#include "StateManager.h"

// ===========================
// System States
// ===========================

class NormalOff : public State {
public:
  NormalOff();
  void tap(StateManager *sm);
  void press(StateManager *sm);
  void update(StateManager *sm);
};

class FallingDot : public State {
public:
  FallingDot();
  void tap(StateManager *sm);
  void update(StateManager *sm);
};

class MiddleOut : public State {
public:
  MiddleOut();
  void tap(StateManager *sm);
  void update(StateManager *sm);
};

class Ripple : public State {
public:
  Ripple();
  void tap(StateManager *sm);
  void update(StateManager *sm);
};

class BangAndFade : public State {
public:
  BangAndFade();
  void tap(StateManager *sm);
  void update(StateManager *sm);
};

class Rainbow : public State {
public:
  Rainbow();
  void tap(StateManager *sm);
  void update(StateManager *sm);
};

class LampMode : public State {
public:
  LampMode();
  void tap(StateManager *sm);
  void press(StateManager *sm);
  void update(StateManager *sm);
};

#endif
