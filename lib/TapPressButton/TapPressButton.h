#ifndef TapPressButton_h
#define TapPressButton_h

class TapPressButton {
public:
  TapPressButton();
  void update(bool input, unsigned long timerVal);
  bool isTap();
  bool isPress();
  
private:
  bool currentButtonState;
  bool prevButtonState;
  bool stateHasChanged();
  unsigned long pressTime;
  unsigned long prevTimerVal;
  unsigned long tapThreshold;
  unsigned long tapLength;
  bool isPressInTapWindow();
  bool isPressInPressWindow();
  unsigned long pressThreshold;
  int pressLength;
  void setStates(bool btnInput);
  void setPressType();
  char pressType;
};

#endif
