#ifndef TapPressButton_h
#define TapPressButton_h

class TapPressButton {
public:
  TapPressButton();
  void updateInput(bool input, unsigned long timerVal);
  bool isPressInPressWindow();
  bool isTap();
  bool isPress();
  int getPressCount();
  
private:
  bool currentButtonState;
  bool prevButtonState;
  bool stateHasChanged();
  unsigned long pressTime;
  unsigned long prevTimerVal;
  unsigned long tapThreshold;
  unsigned long tapLength;
  bool isPressInTapWindow();
  unsigned long pressThreshold;
  int pressLength;
  void setStates(bool btnInput);
  void setPressType();
  char pressType;
};

#endif
