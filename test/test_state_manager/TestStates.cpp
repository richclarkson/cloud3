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

TestOne::TestOne(StateManager *sm) : State(sm) {
  sm->getCurrent()->testVal = 1;
  cout << "\ntest ONE created\n";
}
TestTwo::TestTwo(StateManager *sm) : State(sm) {
  sm->getCurrent()->testVal = 2;
  cout << "\ntest TWO created\n";
}

ColorOne::ColorOne(StateManager *sm) : State(sm) {
  sm->getColor()->testVal = 10;
  cout << "\ncolor ONE created\n";
}

ColorTwo::ColorTwo(StateManager *sm) : State(sm) {
  sm->getColor()->testVal = 20;
  cout << "\ncolor TWO created\n";
}

void TestOne::tap(StateManager *sm) {
  delete this;
  sm->setCurrent(new TestTwo);
}

void TestTwo::tap(StateManager *sm) {
  delete this;
  sm->setCurrent(new TestOne);
}

void TestTwo::press(StateManager *sm) {
  gsm->advanceColor();
}

void ColorOne::press(StateManager *sm) {
  delete this;
  gsm->setColor(new ColorTwo(gsm));
}

void ColorTwo::press(StateManager *sm) {
  delete this;
  gsm->setColor(new ColorOne(gsm));
}
