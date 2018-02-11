#ifndef TestStates_h
#define TestStates_h

#include "StateManager.h"

#include <iostream>
using namespace std;

class TestStateManager : public StateManager {
  public:
  int testVal;
};

class TestStateOne : public State {
public:
  TestStateOne();
  TestStateOne(TestStateManager *sm);
  void tap();
  void press();
};

class TestStateTwo : public State {
public:
  TestStateTwo();
  TestStateTwo(TestStateManager *sm);
  void tap();
  void press();
};

class ColorStateOne : public State {
public:
  ColorStateOne();
  ColorStateOne(TestStateManager *sm);
  void press();
};

class ColorStateTwo : public State {
public:
  ColorStateTwo();
  ColorStateTwo(TestStateManager *sm);
  void press();
};

#endif
