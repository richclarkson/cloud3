#ifndef TapPressButton_h
#define TapPressButton_h

class TapPressButton {
public:
  void updateInput(bool input);
  bool getCurrentState();
  bool hasChanged();
private:
  bool currentButtonState;
  bool prevButtonState;
};

#endif
