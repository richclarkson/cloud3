#ifndef TapPressButton_h
#define TapPressButton_h

class TapPressButton {
public:
  TapPressButton();
  void updateInput(bool input, unsigned long timerVal);
  bool getCurrentState();
  bool stateHasChanged();
  unsigned long getPressTime();
  bool isPressInTapWindow();
  bool isPressInPressWindow();
  bool isTap();
private:
  bool currentButtonState;
  bool prevButtonState;
  unsigned long pressTime;
  unsigned long prevTimerVal;
  unsigned long tapThreshold;
  unsigned long tapLength;
  unsigned long pressThreshold;
  void setStates(bool btnInput);
  void setPressType();
  char pressType;
};

#endif
