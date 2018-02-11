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

void TestOne::tap() {
  delete this;
  gsm->setCurrent(new TestTwo(gsm));
}

void TestTwo::tap() {
  delete this;
  gsm->setCurrent(new TestOne(gsm));
}

void TestTwo::press() {
  gsm->advanceColor();
}

void ColorOne::press() {
  delete this;
  gsm->setColor(new ColorTwo(gsm));
}

void ColorTwo::press() {
  delete this;
  gsm->setColor(new ColorOne(gsm));
}
