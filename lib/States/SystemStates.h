#ifndef SystemStates_h
#define SystemStates_h

#include "StateManager.h"

// ===========================
// System States
// ===========================

class NormalOff : public State {
public:
  NormalOff();
  ~NormalOff(){};
  void tap(StateManager *sm);
  void press(StateManager *sm);
  void update(DisplayController *dc); 
};

class FallingDot : public State {
public:
  FallingDot();
  ~FallingDot(){};
  void tap(StateManager *sm);
  void update(DisplayController *dc);
};

class MiddleOut : public State {
public:
  MiddleOut();
  ~MiddleOut(){};
  void tap(StateManager *sm);
  void update(DisplayController *dc);
};

class Ripple : public State {
public:
  Ripple();
  ~Ripple(){};
  void tap(StateManager *sm);
  void update(DisplayController *dc);
};

class BangAndFade : public State {
public:
  BangAndFade();
  ~BangAndFade(){};
  void tap(StateManager *sm);
  void update(DisplayController *dc);
};

class Rainbow : public State {
public:
  Rainbow();
  ~Rainbow(){};
  void tap(StateManager *sm);
  void update(DisplayController *dc);
};

class LampMode : public State {
public:
  LampMode();
  ~LampMode(){};
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

#endif
