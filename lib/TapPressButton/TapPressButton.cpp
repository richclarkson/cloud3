/*
TapPressButton.cpp - A library to read the button for the Saber 
music lighting product
Created by Steve Faletti, 2017-09-30
Proprietary to Richard Clarkson Studio
*/

#include "Arduino.h"
#include "TapPressButton.h"

TapPressButton::TapPressButton(int pin)
{
	pinMode(pin, INPUT);
	_pin = pin;
	_prevMillis = 0;
	_pressMillis = 0;
	_pressType = 0; // 0:no press, 1: tap, 2: press
	tapThreshold = 50;
	tapLength = 300;
	pressThreshold = 500;
	pressLength = 1000;
}

void TapPressButton::check()
// This function should be called repeatedly in the main loop. Setting the 
// tapThreshold variable acts as a debouncer.
{
	if (digitalRead(_pin) == HIGH)
	{
		_pressMillis += _getPressMillis();
		if (_pressMillis > pressThreshold)
		{
			_setPressType(2); // press press type recorded
		}
	}
	else // _pin is LOW
	{
		// too short for a tap (bouncing), or between a tap and press
		if (_pressMillis <= tapThreshold || (_pressMillis > tapLength + tapThreshold && _pressMillis <= pressThreshold))
		{
			_setPressType(0); // no press recorded
		}
		// tap recorded
		else if (_pressMillis > tapThreshold && _pressMillis <= tapLength)
		{
			_setPressType(1); // tap press type recorded
		}
	}
}

char TapPressButton::getPressType()
{
	char pressTypeHolder = _pressType;
	_pressType = 0;
	return pressTypeHolder;
}

unsigned long TapPressButton::getPressLength() {
	unsigned long pressMillisHolder = _pressMillis;
	_pressMillis = 0;
	return pressMillisHolder;	
}

char TapPressButton::_setPressType(char pressType)
// Prevents press type from being changed back to an earlier state type. 
// Buttons can only go from not pressed -> tapped -> pressed.
{
	if (pressType > _pressType) {
		_pressType = pressType;
	}
	return _pressType;
}

void TapPressButton::_clearPrevMillis()
{
	_prevMillis = 0;
}

unsigned long TapPressButton::_getPressMillis()
{
	unsigned long currentMillis = millis();
	unsigned long returnMillis = 0;
	// Check that some time has passed
	if (currentMillis > _prevMillis)
	{
		returnMillis = currentMillis - _prevMillis;
		_prevMillis = currentMillis;
	}
	else
	{
		// in the unlikely case of a millis() overflow this will wait for the counter to restart. This will likely not be noticed in normal use.
	}
	return returnMillis;
}
