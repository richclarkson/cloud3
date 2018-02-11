#ifdef UNIT_TEST

#include "StateManager.h"
#include "States.h"
#include <unity.h>

#include <iostream>
using namespace std;

class TestDisplayController : public DisplayController {
  int testValue;

public:
  TestDisplayController() { testValue = 0; }
  int getTestVal() { return testValue; }
  void turnOff() {
    cout << "\nTurn off called\n";
    // testValue = 0;
  }
  void displayFallingDot() { testValue = 1; }
  void displayMiddleOut() {
    turnOff();
    testValue = 2;
  }
  void displayRipple() {
    turnOff();
    testValue = 3;
  }
  void displayBangAndFade() {
    turnOff();
    testValue = 4;
  }
  void displayRainbow() {
    turnOff();
    testValue = 5;
  }
  void displayNeon() { testValue = 10; }
  void displayWhite() { testValue = 20; }
  void displayOmbre() { testValue = 30; }
  void displayFire() { testValue = 40; }
};

StateManager sm;
TestDisplayController tdc;
unsigned long timerVal;

void setUp() {
  sm = StateManager(new NormalOff, new Neon);
  sm.registerDisplayController(&tdc);
  cout << "\n" << &sm;
  cout << " | " << sm.getCurrent()->getSM() << "\n";
}

void test_normal_off_system_state_calls_display_function() {
  // sm.update();
  // TEST_ASSERT_EQUAL(0, tdc.getTestVal());
  // TEST_ASSERT_EQUAL(0, 0);
}

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_normal_off_system_state_calls_display_function);

  UNITY_END();
}

#endif
