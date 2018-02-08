#ifndef TestStates_h
#define TestStates_h

#include "StateManager.h"

#include <iostream>
using namespace std;

class TestState : public State {
};

class TestStateOne : public TestState {
public:
  TestStateOne();
  ~TestStateOne();
  void tap(StateManager *sm);
};

class TestStateTwo : public TestState {
public:
  TestStateTwo();
  ~TestStateTwo();
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class TestStateThree : public TestState {
public:
  TestStateThree();
  void press(StateManager *sm);
  void tap(StateManager *sm);
};

class ColorState : public TestState {
public:
  void advance(StateManager *sm) {}
};

class ColorStateOne : public ColorState {
public:
  ColorStateOne();
  void press(StateManager *sm);
};

class ColorStateTwo : public ColorState {
public:
  ColorStateTwo();
  void press(StateManager *sm);
};


#endif
