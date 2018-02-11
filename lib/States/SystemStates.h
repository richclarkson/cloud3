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
  void tap();
  void press();
  void update();
};

class FallingDot : public State {
public:
  FallingDot();
  FallingDot(StateManager *sm);
  void tap();
  void update();
};

class MiddleOut : public State {
public:
  MiddleOut();
  MiddleOut(StateManager *sm);
  void tap();
  void update();
};

class Ripple : public State {
public:
  Ripple();
  Ripple(StateManager *sm);
  void tap();
  void update();
};

class BangAndFade : public State {
public:
  BangAndFade();
  BangAndFade(StateManager *sm);
  void tap();
  void update();
};

class Rainbow : public State {
public:
  Rainbow();
  Rainbow(StateManager *sm);
  void tap();
  void update();
};

class LampMode : public State {
public:
  LampMode();
  LampMode(StateManager *sm);
  void tap();
  void press();
  void update();
};

#endif
