#ifndef TapPressButton_h
#define TapPressButton_h

class TapPressButton {
public:
  TapPressButton();
  void updateInput(bool input, unsigned long timerVal);
  bool getCurrentState();
  bool hasStateChanged();
  unsigned long getPressTime();
  bool isPressInTapWindow();
private:
  bool currentButtonState;
  bool prevButtonState;
  unsigned long pressTime;
  unsigned long tapThreshold;
  unsigned long tapLength;
};

#endif
