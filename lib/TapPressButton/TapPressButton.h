#ifndef TapPressButton_h
#define TapPressButton_h

class TapPressButton {
public:
  TapPressButton();
  void update(bool btnVal, unsigned long timerVal);
  bool isTap();
  bool isPress();
  void setDebounce(unsigned long debounceTime);
  void setTapLength(unsigned long tapTime);
  void setPressThreshold(unsigned long pressThreshold);
  void setPressLength(unsigned long pressLength);

private:
  unsigned long prevTimeStamp;
  unsigned long pressTime;
  bool buttonState;
  unsigned long DEBOUNCE_THRESHOLD;
  unsigned long TAP_LENGTH;
  unsigned long PRESS_THRESHOLD;
  unsigned long PRESS_LENGTH;
  void updatePressTime(unsigned long newTime);
  int pressCount;
  int getPressCount();
  void resetPressTime();
  bool isDebounced();
};

#endif
