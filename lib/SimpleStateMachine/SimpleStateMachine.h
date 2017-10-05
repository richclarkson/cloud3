/* 
Simple State Machine
A very simple finite state machine library
Created by Steve Faletti, 2017-10-01
Proprietary to Richard Clarkson Studio
*/

#ifndef SimpleStateMachine_h
#define SimpleStateMachine_h

class SimpleStateMachine
{
public:
  SimpleStateMachine(int statesCount);
  int getCurrent();
  void setCurrent(int newState);
  void next();

private:
  int _currentState;
  int _statesCount;
};

#endif