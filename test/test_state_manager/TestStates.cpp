#include "TestStates.h"

#include <iostream>
using namespace std;

TestStateOne::TestStateOne() : State() {
  // cout << "\nTest State One created; SM: NO\n";
}

TestStateOne::TestStateOne(TestStateManager *sm) : State(sm) {
  sm->testVal = 1;
  // cout << "\nTest State One created; SM: YES\n";
}

TestStateTwo::TestStateTwo() : State() {
  // cout << "\nTest State Two created; SM: NO\t" << this << "\n";
}

TestStateTwo::TestStateTwo(TestStateManager *sm) : State(sm) {
  sm->testVal = 2;
  // cout << "\nTest State Two created; SM: YES\t" << this << "\n";
}

ColorStateOne::ColorStateOne() : State() {}

ColorStateOne::ColorStateOne(TestStateManager *sm) : State(sm) { sm->testVal = 10; }

ColorStateTwo::ColorStateTwo() : State() {}

ColorStateTwo::ColorStateTwo(TestStateManager *sm) : State(sm) { sm->testVal = 20; }

void TestStateOne::tap() {
  this->gsm->setCurrent(new TestStateTwo(this->gsm));
  delete this;
}

void TestStateTwo::tap() {
  this->gsm->setCurrent(new TestStateOne(this->gsm));
  delete this;
}

void TestStateOne::press() { this->gsm->advanceColor(); }

void TestStateTwo::press() { this->gsm->advanceColor(); }

void ColorStateOne::press() {
  this->gsm->setColor(new ColorStateTwo(this->gsm));
}

void ColorStateTwo::press() {
  this->gsm->setColor(new ColorStateOne(this->gsm));
}
