#ifndef SaberDisplayController_h
#define SaberDisplayController_h

#include "StateManager.h"

class SaberDisplayController : public DisplayController {
  bool whiteFlag;
  unsigned long timerVal;
  unsigned long prevTimerVal;

public:
  SaberDisplayController();

  void turnOff();
  void update(StateManager *sm);

  bool isTimeFrame(unsigned long newTimer, unsigned long timeFrame);
  
  void displayFallingDot();
  void displayMiddleOut();
  void displayRipple();
  void displayBangAndFade();
  void displayRainbow();
  
  void displayNeon();
  void displayWhite();
  void displayOmbre();
  void displayFire();
};

#endif
