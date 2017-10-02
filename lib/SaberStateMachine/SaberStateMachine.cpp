/* 
Saber State Machine
A very simple finite state machine library
Created by Steve Faletti, 2017-10-01
Proprietary to Richard Clarkson Studio
*/

#include "SaberStateMachine.h"

SaberStateMachine::SaberStateMachine(int statesCount)
{
    _currentState = 0;
    _statesCount = statesCount;
}

int SaberStateMachine::getCurrent()
{
    return _currentState;
}

void SaberStateMachine::setCurrent(int newState)
{
   if (newState >= _statesCount)
   {
       newState = _statesCount - 1;
   }
    _currentState = newState;
}

void SaberStateMachine::next()
// Increment the state. Rollover to zero if it is at max.
{
    _currentState ++;
    if (_currentState >= _statesCount)
    {
        _currentState = 0;
    }
    
}
