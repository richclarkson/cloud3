#include "TestStates.h"

#include <iostream>
using namespace std;

TestOne::TestOne() : State() {
  State::testVal = 1;
  cout << "\ntest One created\n";
}
TestTwo::TestTwo() : State() {
  State::testVal = 2;
  cout << "\ntest Two created\n";
}

ColorOne::ColorOne() : State() {
  State::testVal = 10;
  cout << "\ncolor One created\n";
}

ColorTwo::ColorTwo() : State() {
  State::testVal = 20;
  cout << "\ncolor Two created\n";
}

void TestOne::tap(StateManager *sm) {
  delete this;
  sm->setCurrent(new TestTwo);
}

void TestTwo::tap(StateManager *sm) {
  delete this;
  sm->setCurrent(new TestOne);
}

void TestTwo::press(StateManager *sm) { sm->advanceColor(); }

void ColorOne::press(StateManager *sm) {
  delete this;
  sm->setColor(new ColorTwo);
}

void ColorTwo::press(StateManager *sm) {
  delete this;
  sm->setColor(new ColorOne);
}
