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
  int channel;
  int sensitvity;
  int brightness;

public:
  StateManager();
  void setCurrent(State *s) { current = s; }
  void setColor(State *s) { colorSetting = s; }
  void advanceColor();
  void advanceChannel();
  void advanceSensitivity();
  void advanceBrightness();
  void resetSettings();
  void tap();
  void press();
};

class State {
public:
  virtual ~State(){};
  virtual void tap(StateManager *sm){};
  virtual void press(StateManager *sm){};
};

// ===========================
// Declare system states here
// ===========================

class NormalOff : public State {
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
  void press(StateManager *sm);
};

// =====================
// Setting Modes
// =====================

class SettingsOff : public State {
public:
  SettingsOff(){};
  ~SettingsOff(){};
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class Channel : public State {
public:
  Channel(){};
  ~Channel(){};
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class Sensitivity : public State {
public:
  Sensitivity(){};
  ~Sensitivity(){};
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class Brightness : public State {
public:
  Brightness(){};
  ~Brightness(){};
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class Reset : public State {
public:
  Reset(){};
  ~Reset(){};
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

// =====================
// Color Settings
// =====================
class Neon : public State {
public:
  Neon(){};
  ~Neon(){};
  void press(StateManager *sm);
};

class White : public State {
public:
  White(){};
  ~White(){};
  void press(StateManager *sm);
};

class Ombre : public State {
public:
  Ombre(){};
  ~Ombre(){};
  void press(StateManager *sm);
};

class Fire : public State {
public:
  Fire(){};
  ~Fire(){};
  void press(StateManager *sm);
};

#endif
