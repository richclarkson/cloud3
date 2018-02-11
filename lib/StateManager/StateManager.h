#ifndef StateManager_h
#define StateManager_h

class StateManager {
  friend class TestStateManager;
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
  State* getCurrent() { return current; }
  int getCurrentID();
  void setColor(State *s);
  State *getColor() { return colorSetting; }
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
  void registerFFT(void (*cb)(), float *fftArrayPointer);
  void callFFTCallback();
  void registerLevel(void (*cb)(), float *levelValPointer);
  void callLevelCallback();
  void update();
  void update(unsigned long timerVal);

  void registerDisplayController(DisplayController *dc);
  DisplayController* getDisplayController() { return display; }
  int getDisplayTest();

};

class State {
public:
  virtual ~State(){};
  virtual void tap(StateManager *sm){};
  virtual void press(StateManager *sm){};
  virtual void update(StateManager *sm){};
  int getID() { return id; }

protected:
  int id;
};

class DisplayController {
public:
  virtual void turnOff() {}

  virtual void displayFallingDot() {}
  virtual void displayMiddleOut() {}
  virtual void displayRipple() {}
  virtual void displayBangAndFade() {}
  virtual void displayRainbow() {}

};

#endif
