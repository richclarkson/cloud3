#ifndef TapPressButton_h
#define TapPressButton_h

class TapPressButton {
public:
  TapPressButton();
  void update(bool btnVal, unsigned long timerVal);
  char getPressType();
  unsigned long getPressLength();
  unsigned long setDebounce(unsigned long debounceTime);
  unsigned long setTapLength(unsigned long tapTime);
  unsigned long setPressThreshold(unsigned long pressThreshold);
  unsigned long setPressLength(unsigned long pressLength);

private:
  unsigned long prevTimeStamp;
  unsigned long pressTime;
  char pressType;
  unsigned long DEBOUNCE_THRESHOLD;
  unsigned long TAP_LENGTH;
  unsigned long PRESS_THRESHOLD;
  unsigned long PRESS_LENGTH;
  void updatePressTime(unsigned long newTime);
  bool isTap();
  bool isPress();
  int pressCount;
  int getPressCount();
  void resetTimeStamp();
  void resetPressTime();
  bool isDebounced();
};

#endif
