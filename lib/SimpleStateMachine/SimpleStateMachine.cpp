/* 
Simple State Machine
A very simple finite state machine library
Created by Steve Faletti, 2017-10-01
Proprietary to Richard Clarkson Studio
*/

#include "SimpleStateMachine.h"

SimpleStateMachine::SimpleStateMachine(int statesCount)
{
    _currentState = 0;
    _statesCount = statesCount;
}

int SimpleStateMachine::getCurrent()
{
    return _currentState;
}

void SimpleStateMachine::setCurrent(int newState)
{
   if (newState >= _statesCount)
   {
       newState = _statesCount - 1;
   }
    _currentState = newState;
}

void SimpleStateMachine::next()
// Increment the state. Rollover to zero if it is at max.
{
    _currentState ++;
    if (_currentState >= _statesCount)
    {
        _currentState = 0;
    }
    
}
