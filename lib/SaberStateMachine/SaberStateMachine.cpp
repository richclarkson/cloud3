/* 
Saber State Machine
A very simple finite state machine library
Created by Steve Faletti, 2017-10-01
Proprietary to Richard Clarkson Studio
*/

#include "SaberStateMachine.h"

SaberStateMachine::SaberStateMachine()
{
    _currentState = OFF;
}

int SaberStateMachine::getCurrent()
{
    return _currentState;
}

void SaberStateMachine::setCurrent(int newState)
{
    _currentState = newState;
}

void SaberStateMachine::next()
{
    _currentState ++;
    if (_currentState >= STATES_ENUM_LENGTH)
    {
        _currentState = OFF;
    }
    
}
