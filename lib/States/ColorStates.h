#ifndef ColorStates_h
#define ColorStates_h

#include "StateManager.h"

// =====================
// Color States
// =====================

class Neon : public State {
public:
  Neon();
  Neon(StateManager *sm);
  void press(StateManager *sm);
  void update(StateManager *sm);
};

class White : public State {
public:
  White();
  White(StateManager *sm);
  void press(StateManager *sm);
  void update(StateManager *sm);
};

class Ombre : public State {
public:
  Ombre();
  Ombre(StateManager *sm);
  void press(StateManager *sm);
  void update(StateManager *sm);
};

class Fire : public State {
public:
  Fire();
  Fire(StateManager *sm);
  void press(StateManager *sm);
  void update(StateManager *sm);
};

#endif
