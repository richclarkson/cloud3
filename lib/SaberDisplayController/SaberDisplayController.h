#ifndef SaberDisplayController_h
#define SaberDisplayController_h

#include "StateManager.h"

class SaberDisplayController : public DisplayController {
  bool whiteFlag;

public:
  SaberDisplayController();

  void turnOff();
  void update();
  
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
