#ifndef SystemStates_h
#define SystemStates_h

#include "StateManager.h"

// ===========================
// System States
// ===========================

class NormalOff : public State {
public:
  NormalOff();
  NormalOff(StateManager *sm);
  void tap(StateManager *sm);
  void press(StateManager *sm);
  void update(StateManager *sm);
};

class FallingDot : public State {
public:
  FallingDot();
  FallingDot(StateManager *sm);
  void tap(StateManager *sm);
  void update(StateManager *sm);
};

class MiddleOut : public State {
public:
  MiddleOut();
  MiddleOut(StateManager *sm);
  void tap(StateManager *sm);
  void update(StateManager *sm);
};

class Ripple : public State {
public:
  Ripple();
  Ripple(StateManager *sm);
  void tap(StateManager *sm);
  void update(StateManager *sm);
};

class BangAndFade : public State {
public:
  BangAndFade();
  BangAndFade(StateManager *sm);
  void tap(StateManager *sm);
  void update(StateManager *sm);
};

class Rainbow : public State {
public:
  Rainbow();
  Rainbow(StateManager *sm);
  void tap(StateManager *sm);
  void update(StateManager *sm);
};

class LampMode : public State {
public:
  LampMode();
  LampMode(StateManager *sm);
  void tap(StateManager *sm);
  void press(StateManager *sm);
  void update(StateManager *sm);
};

#endif
