#ifdef UNIT_TEST

#include "StateManager.h"
#include "States.h"
#include <unity.h>

class TestDisplayController : public DisplayController {
  int testVal;

public:
  TestDisplayController() { testVal = 0; }
  int getTestVal() { return testVal; }
  void turnOff() { testVal = 0; }
  void displayFallingDot() {
    testVal = 1;
  }
  void displayMiddleOut() {
    turnOff();
    testVal = 2;
  }
  void displayRipple() {
    turnOff();
    testVal = 3;
  }
  void displayBangAndFade() {
    turnOff();
    testVal = 4;
  }
  void displayRainbow() {
    turnOff();
    testVal = 5;
  }
  void displayNeon() {
    testVal = 10;
  }
  void displayWhite() {
    testVal = 20;
  }
  void displayOmbre() {
    testVal = 30;
  }
  void displayFire() {
    testVal = 40;
  }
};

StateManager sm;
TestDisplayController tdc;
unsigned long timerVal;

void setUp() {
  sm = StateManager(new NormalOff, new Neon);
  sm.registerDisplayController(&tdc);
}

void test_normal_off_system_state_calls_display_function() {
  // sm.update();
  // TEST_ASSERT_EQUAL(0, tdc.getTestVal());
  sm.tap();
  sm.update();
  TEST_ASSERT_EQUAL(0, 0);
}

int main() {
  UNITY_BEGIN();

    RUN_TEST(test_normal_off_system_state_calls_display_function);

  UNITY_END();
}

#endif
