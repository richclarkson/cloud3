#ifndef TestStates_h
#define TestStates_h

#include "StateManager.h"

#include <iostream>
using namespace std;

class TestOne : public State {
public:
  TestOne();
  void tap(StateManager *sm);
  int testVal;
};

class TestTwo : public State {
public:
  TestTwo();
  void tap(StateManager *sm);
  void press(StateManager *sm);
  int testVal;
};

class ColorOne : public State {
public:
  ColorOne();
  void press(StateManager *sm);
  int testVal;
};

class ColorTwo : public State {
public:
  ColorTwo();
  void press(StateManager *sm);
  int testVal;
};

#endif
