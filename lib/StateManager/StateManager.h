#ifndef StateManager_h
#define StateManager_h

// =======================
// Class definitions
// =======================

class StateManager {
  class State *current;
  class State *colorSetting;
  int channel;
  int sensitivity;
  int brightness;
  void (*fftCallBack) ();
  void (*levelCallback) ();
  float* levelValPtr;
  float* fftArrayPtr;


public:
  StateManager();
  void setCurrent(State *s) { current = s; }
  void setColor(State *s) { colorSetting = s; }
  void advanceColor();
  void advanceChannel();
  int getChannel() { return channel; }
  int getSensitivity() { return sensitivity; }
  int getBrightness() { return brightness; }
  void advanceSensitivity();
  void advanceBrightness();
  void resetSettings();
  void tap();
  void press();
  void registerFFTCallback(void (*cb)(), float* fftArrayPointer);
  void callFFTCallback();
  void registerLevelCallback(void (*cb)(), float* levelValPointer);
  void callLevelCallback();
  void update();
};

class State {
public:
  virtual ~State(){};
  virtual void tap(StateManager *sm){};
  virtual void press(StateManager *sm){};
  virtual void update(StateManager *sm){};
};

// ===========================
// Declare system states here
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
// Setting Modes
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
// Color Settings
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
