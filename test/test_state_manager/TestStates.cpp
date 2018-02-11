#include "TestStates.h"

#include <iostream>
using namespace std;

TestOne::TestOne() : State() {
  State::testVal = 1;
  cout << "\ntest One created\n";
}
TestTwo::TestTwo() : State() {
  cout << "\ntest Two created\n";
}

TestOne::TestOne(StateManager *sm) : State(sm) {
  sm->getCurrent()->testVal = 1;
  cout << "\ntest ONE created\n";
}
TestTwo::TestTwo(StateManager *sm) : State(sm) {
  sm->getCurrent()->testVal = 2;
  cout << "\ntest TWO created\n";
}

void TestOne::tap() {
  gsm->setCurrent(new TestTwo(gsm));
  delete this;
}

void TestTwo::tap() {
  gsm->setCurrent(new TestOne(gsm));
  delete this;
}

