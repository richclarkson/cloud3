#ifndef ColorStates_h
#define ColorStates_h

#include "StateManager.h"

// =====================
// Color States
// =====================

class Neon : public State {
public:
  Neon();
  ~Neon(){};
  void press(StateManager *sm);
};

class White : public State {
public:
  White();
  ~White(){};
  void press(StateManager *sm);
};

class Ombre : public State {
public:
  Ombre();
  ~Ombre(){};
  void press(StateManager *sm);
};

class Fire : public State {
public:
  Fire();
  ~Fire(){};
  void press(StateManager *sm);
};

#endif
