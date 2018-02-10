#ifndef StateManager_h
#define StateManager_h

class StateManager {
  friend class TestStateManager;
  class State *current;
  class State *colorSetting;
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
  int getCurrentID();
  void setColor(State *s);
  State* getColor() { return colorSetting; }
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

};

#endif
