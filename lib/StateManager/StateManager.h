#ifndef StateManager_h
#define StateManager_h

class StateManager {
  class State *current;
  class State *colorSetting;
  class DisplayController *display;
  int channel;
  int sensitivity;
  int brightness;
  void (*fftCallBack)();
  void (*levelCallback)();
  float *levelValPtr;
  float *fftArrayPtr;

public:
  StateManager();
  StateManager(State *starting, State *startingColor);
  void setCurrent(State *s);
  void setColor(State *s);

  State *getCurrent();
  State *getColor();
  DisplayController *getDisplayController();
  int getChannel();
  int getSensitivity();
  int getBrightness();

  void registerDisplayController(DisplayController *dc);
  void registerFFT(void (*cb)(), float *fftArrayPointer);
  void registerLevel(void (*cb)(), float *levelValPointer);

  void advanceColor();
  void advanceChannel();
  void advanceSensitivity();
  void advanceBrightness();
  void resetSettings();
  void tap();
  void press();
  void updateFFT();
  void updateLevel();
  void update();
};

class State {
protected:
  int testVal;

public:
  State() {}
  virtual void tap(StateManager *sm) {}
  virtual void press(StateManager *sm) {}
  virtual void update(StateManager *sm) {}
  virtual void setTestVal(int val) {}
  int getTestVal() { return testVal; }
};

class DisplayController {
public:
  virtual void turnOff() {}
  virtual void update(StateManager *sm) {}

  virtual void displayFallingDot() {}
  virtual void displayMiddleOut() {}
  virtual void displayRipple() {}
  virtual void displayBangAndFade() {}
  virtual void displayRainbow() {}

  virtual void displayNeon() {}
  virtual void displayWhite() {}
  virtual void displayOmbre() {}
  virtual void displayFire() {}
};

#endif
