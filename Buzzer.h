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
		this->pin = pin;
	}
	void PlayShort()
	{
		tone(pin, 2500, 100);
	}
	void PlayLong()
	{
		tone(pin, 2000, 500);
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

