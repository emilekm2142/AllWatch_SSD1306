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

 public:
	Buzzer(int pin)
	{
		pinMode(pin, OUTPUT);
		this->pin = pin;
		StopAll();
	}
	void PlayShort()
	{
		tone(pin, 2500, 20);
	}
	void PlayLong()
	{
		tone(pin, 2000, 100);
	}
	void Play(unsigned long length, int f=30)
	{
		tone(pin, f, length);
	}
	void StopAll()
	{
		noTone(pin);
	}
};



#endif

