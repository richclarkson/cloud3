#ifndef TestStates_h
#define TestStates_h

#include "StateManager.h"

#include <iostream>
using namespace std;

class TestOne : public State {
public:
  TestOne();
  TestOne(StateManager *sm);
  void tap();
  int testVal;
};

class TestTwo : public State {
public:
  TestTwo();
  TestTwo(StateManager *sm);
  void tap();
  void press(StateManager *sm);
  int testVal;
};

class ColorOne : public State {
public:
  ColorOne();
  ColorOne(StateManager *sm);
  void press(StateManager *sm);
  int testVal;
};

class ColorTwo : public State {
public:
  ColorTwo();
  ColorTwo(StateManager *sm);
  void press(StateManager *sm);
  int testVal;
};

#endif
