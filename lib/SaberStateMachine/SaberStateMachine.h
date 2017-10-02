/* 
Saber State Machine
A very simple finite state machine library
Created by Steve Faletti, 2017-10-01
Proprietary to Richard Clarkson Studio
*/

#ifndef SaberStateMachine_h
#define SaberStateMachine_h

class SaberStateMachine
{
public:
  SaberStateMachine(int statesCount);
  int getCurrent();
  void setCurrent(int newState);
  void next();

private:
  int _currentState;
  int _statesCount;
};

#endif