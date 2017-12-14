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
  unsigned long TAP_THRESHOLD;
  unsigned long TAP_LENGTH;
  unsigned long PRESS_THRESHOLD;
  unsigned long pressTime;
  unsigned long prevTimerVal;
  bool isPressInTapWindow();
  bool isPressInPressWindow();
  void setStates(bool btnInput);
  void setPressType();
  char pressType;
};

#endif
