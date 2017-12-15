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
  unsigned long PRESS_LENGTH;
  unsigned long tapTimer;
  unsigned long pressTimer;
  unsigned long prevTimerVal;
  bool isPressInTapWindow();
  bool isPressInPressWindow();
  void setStates(bool btnInput);
  void setPressType();
  char pressType;
  bool pressTypeIsTap();
  bool pressTypeIsPress();
  void setTimers(unsigned long timerVal);
  void setPressTimer(unsigned long TimerVal);
  void setTapTimer(unsigned long TimerVal);
};

#endif
