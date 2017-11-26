#ifndef SaberStateHandler_h
#define SaberStateHandler_h

#include "BasicStateMachine.h"

class SaberStateHandler {
public:
  SaberStateHandler();
  int getMainState();
  int getNormalMode();
  void tap();
  void press();

private:
  BasicStateMachine mainState;
  BasicStateMachine normalModes;
  BasicStateMachine settingModes;
};

#endif
