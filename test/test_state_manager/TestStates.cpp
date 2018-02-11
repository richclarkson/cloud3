#include "TestStates.h"

#include <iostream>
using namespace std;

TestState::TestState() : State() {}
TestState::TestState(StateManager *sm) : State(sm) {}

TestStateOne::TestStateOne() : TestState() {
  this->testVal = 1;
  // cout << "\nTest State One created; SM: NO\n";
}

TestStateOne::TestStateOne(StateManager *sm) : TestState(sm) {
  this->testVal = 1;
  // cout << "\nTest State One created; SM: YES\n";
}

TestStateTwo::TestStateTwo() : TestState() {
  this->testVal = 2;
  // cout << "\nTest State Two created; SM: NO\t" << this << "\n";
}

TestStateTwo::TestStateTwo(StateManager *sm) : TestState(sm) {
  this->testVal = 2;
  // cout << "\nTest State Two created; SM: YES\t" << this << "\n";
}

ColorStateOne::ColorStateOne() : TestState() { this->testVal = 10; }

ColorStateOne::ColorStateOne(StateManager *sm) : TestState(sm) {
  this->testVal = 10;
}

ColorStateTwo::ColorStateTwo() : TestState() { this->testVal = 20; }

ColorStateTwo::ColorStateTwo(StateManager *sm) : TestState(sm) {
  this->testVal = 20;
}

void TestStateOne::tap() {
  StateManager *sm = gsm;
  delete this;
  sm->setCurrent(new TestStateTwo(sm));
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
