#ifndef States_h
#define States_h

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
};

class FallingDot : public State {
public:
  FallingDot();
  ~FallingDot(){};
  void tap(StateManager *sm);
  void update(StateManager *sm);
};

class MiddleOut : public State {
public:
  MiddleOut();
  ~MiddleOut(){};
  void tap(StateManager *sm);
  void update(StateManager *sm);
};

class Ripple : public State {
public:
  Ripple();
  ~Ripple(){};
  void tap(StateManager *sm);
  void update(StateManager *sm);
};

class BangAndFade : public State {
public:
  BangAndFade();
  ~BangAndFade(){};
  void tap(StateManager *sm);
  void update(StateManager *sm);
};

class Rainbow : public State {
public:
  Rainbow();
  ~Rainbow(){};
  void tap(StateManager *sm);
  void update(StateManager *sm);
};

class LampMode : public State {
public:
  LampMode();
  ~LampMode(){};
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

// =====================
// Setting States
// =====================

class SettingsOff : public State {
public:
  SettingsOff();
  ~SettingsOff(){};
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class Channel : public State {
public:
  Channel();
  ~Channel(){};
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class Sensitivity : public State {
public:
  Sensitivity();
  ~Sensitivity(){};
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class Brightness : public State {
public:
  Brightness();
  ~Brightness(){};
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class Reset : public State {
public:
  Reset();
  ~Reset(){};
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

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