#ifndef TapPressButton_h
#define TapPressButton_h

class TapPressButton {
public:
  TapPressButton();
  void update(bool btnVal, unsigned long timerVal);
  bool isTap();
  bool isPress();
  int getPressCount();
  void setDebounce(unsigned long debounceTime);
  void setTapLength(unsigned long tapTime);
  void setPressThreshold(unsigned long pressThreshold);
  void setPressLength(unsigned long pressLength);

private:
  unsigned long prevTimeStamp;
  unsigned long pressTime;
  void updatePressTime(unsigned long newTime);
  void resetPressTime();
  char pressType;
  bool isPressInTapWindow();
  bool isPressInPressWindow();
  unsigned long DEBOUNCE_THRESHOLD;
  unsigned long TAP_LENGTH;
  unsigned long PRESS_THRESHOLD;
  unsigned long PRESS_LENGTH;
  int pressCount;
  bool prevButtonState;
};

#endif
