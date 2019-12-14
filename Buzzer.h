// Buzzer.h

#ifndef _BUZZER_h
#define _BUZZER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Buzzer
{
 protected:
	 int pin;
	 bool enabled = false;

 public:
	Buzzer(int pin)
	{
		if (!enabled) return;
		pinMode(pin, OUTPUT);
		this->pin = pin;
		StopAll();
	}
	void PlayShort()
	{
		if (!enabled) return;
		tone(pin, 2500, 20);
	}
	void PlayLong()
	{
		if (!enabled) return;
		tone(pin, 2000, 100);
	}
	void Play(unsigned long length, int f=30)
	{
		if (!enabled) return;
		tone(pin, f, length);
	}
	void StopAll()
	{
		if (!enabled) return;
		noTone(pin);
	}
};



#endif

