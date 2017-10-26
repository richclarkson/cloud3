#ifndef TapPressButton_h
#define TapPressButton_h

class TapPressButton {
public:
  void updateInput(bool input, unsigned long timerVal);
  bool getCurrentState();
  bool hasStateChanged();
  unsigned long getPressTime();
private:
  bool currentButtonState;
  bool prevButtonState;
  unsigned long pressTime;
};

#endif
