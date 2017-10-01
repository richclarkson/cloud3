/*
SaberButton.cpp - A library to read the button for the Saber music lighting product
Created by Steve Faletti, 2017-09-30
Proprietary to Richard Clarkson Studio
*/

#include "Arduino.h"
#include "SaberButton.h"

SaberButton::SaberButton(int pin) {
	pinMode(pin, INPUT);
	_pin = pin;	
	_prevMillis = 0;
	_pressMillis = 0;
}

// This function should be called repeatedly in the main loop. It assumes a debounced button, though in actual application it will just wait until the button has settled before beginning to count, which won't be noticed by most users.
long SaberButton::getPressTime() {
	if (digitalRead(_pin) == HIGH) {
		long millis = millis();
		if (millis > _prevMillis) {
			_pressMillis += millis - _prevMillis;
			_prevMillis = millis
		}
		else {
			// in the unlikely case of a millis() overflow this will wait for the counter to restart. This will likely not be noticed in normal use.
		}
	}
	else {
		_pressMillis = 0;
		_prevMillis = 0;
	}
	return _pressMillis;
}