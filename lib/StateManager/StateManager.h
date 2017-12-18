#ifndef StateManager_h
#define StateManager_h

#include <iostream>
using namespace std;

class StateManager {
  class State *current;
  class State *colorSetting;

public:
  StateManager();
  void setCurrent(State *s) { current = s; }
  void setColor(State *s) { colorSetting = s; }
  void advanceColor();
  void tap();
  void press();
};

class State {
public:
  virtual void tap(StateManager *sm) {}
  virtual void press(StateManager *sm) {}
};

// ===========================
// Declare system states here
// ===========================

class NormalOff : public State {
public:
  NormalOff() { cout << "Normal Off Entered\n"; }
  ~NormalOff(){};
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class FallingDot : public State {
public:
  FallingDot() { cout << "Falling Dot Entered\n"; }
  ~FallingDot(){};
  void tap(StateManager *sm);
};

class MiddleOut : public State {
public:
  MiddleOut() { cout << "Middle Out Entered\n"; }
  ~MiddleOut(){};
  void tap(StateManager *sm);
};

class Ripple : public State {
public:
  Ripple() { cout << "Ripple Entered\n"; }
  ~Ripple(){};
  void tap(StateManager *sm);
};

class BangAndFade : public State {
public:
  BangAndFade() { cout << "Bang and Fade Entered\n"; }
  ~BangAndFade(){};
  void tap(StateManager *sm);
};

class Rainbow : public State {
public:
  Rainbow() { cout << "Rainbow Entered\n"; }
  ~Rainbow(){};
  void tap(StateManager *sm);
};

class LampMode : public State {
public:
  LampMode() { cout << "Lamp Mode Entered\n"; }
  ~LampMode(){};
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class SettingsOff : public State {
public:
  SettingsOff() { cout << "Settings Off Entered\n"; }
  ~SettingsOff(){};
  void press(StateManager *sm);
};

// =====================
// Color Settings
// =====================
class Neon : public State {
public:
  Neon(){ cout << "Neon Color Selected\n"; }
  ~Neon(){};
  void press(StateManager *sm);
};

class White : public State {
public:
  White() { cout << "White Color Selected\n"; }
  ~White(){};
  void press(StateManager *sm);
};

#endif
