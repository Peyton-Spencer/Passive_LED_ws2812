/*
 * Button.h
 *
 *  Created on: Dec 1, 2017
 *      Author: pas12_000
 */
#include <Arduino.h>
#ifndef BUTTON_H_
#define BUTTON_H_

class Button { //Class for a button that cycles through a set number of options, each press goes to the next option

	boolean lastState;  //Keeps track of last state of the actual switch realtime
	uint8_t buttonPin;  //Simply the pin of the switch
	unsigned long lastPoll; //last time a state change was read on the input pin, this is used later on

public:
	Button(uint8_t pin)  //Constructor
{
		pinMode(pin, INPUT_PULLUP);
		lastState = HIGH;
		buttonPin = pin;
		lastPoll = millis();
}

	//Returns true iff button goes HIGH to LOW- prevents registering multiple presses
	bool checkState()
	{
		unsigned long currentPoll = millis();
		if (currentPoll - lastPoll >= 15) //this is to prevent over-reading the input if there is a shaky connection that goes on and off a bunch
		{
			lastPoll = currentPoll;
			int currentState = digitalRead(buttonPin);
			if (currentState != lastState)
			{
				lastState = currentState;
				if (lastState == HIGH)
				{
					return true; //from HIGH to LOW
				}
			}
		} return false; //No change or LOW to HIGH
	}
};



#endif /* BUTTON_H_ */
