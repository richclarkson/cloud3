#ifndef StateManager_h
#define StateManager_h

#if defined UNIT_TEST
  #include <iostream>
  using namespace std;
  #define PRINT_MSG(msg) cout << msg << "\n"
#else
  #define PRINT_MSG(msg) 
#endif


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
  NormalOff() { PRINT_MSG("Normal Off Entered"); }
  ~NormalOff(){};
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class FallingDot : public State {
public:
  FallingDot() { PRINT_MSG("Falling Dot Entered"); }
  ~FallingDot(){};
  void tap(StateManager *sm);
};

class MiddleOut : public State {
public:
  MiddleOut() { PRINT_MSG("Middle Out Entered"); }
  ~MiddleOut(){};
  void tap(StateManager *sm);
};

class Ripple : public State {
public:
  Ripple() { PRINT_MSG("Ripple Entered"); }
  ~Ripple(){};
  void tap(StateManager *sm);
};

class BangAndFade : public State {
public:
  BangAndFade() { PRINT_MSG("Bang and Fade Entered"); }
  ~BangAndFade(){};
  void tap(StateManager *sm);
};

class Rainbow : public State {
public:
  Rainbow() { PRINT_MSG("Rainbow Entered"); }
  ~Rainbow(){};
  void tap(StateManager *sm);
};

class LampMode : public State {
public:
  LampMode() { PRINT_MSG("Lamp Mode Entered"); }
  ~LampMode(){};
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class SettingsOff : public State {
public:
  SettingsOff() { PRINT_MSG("Settings Off Entered"); }
  ~SettingsOff(){};
  void press(StateManager *sm);
};

// =====================
// Color Settings
// =====================
class Neon : public State {
public:
  Neon() { PRINT_MSG("Neon Color Selected"); }
  ~Neon(){};
  void press(StateManager *sm);
};

class White : public State {
public:
  White() { PRINT_MSG("White Color Selected"); }
  ~White(){};
  void press(StateManager *sm);
};

class Ombre : public State {
public:
  Ombre() { PRINT_MSG("Ombre Color Selected"); }
  ~Ombre(){};
  void press(StateManager *sm);
};

class Fire : public State {
public:
  Fire() { PRINT_MSG("Fire Color Selected"); }
  ~Fire(){};
  void press(StateManager *sm);
};

#endif
