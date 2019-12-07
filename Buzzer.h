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
		tone(pin, 500, 100);
	}
	void PlayLong()
	{
		tone(pin, 500, 500);
	}
	void Play(unsigned long length)
	{
		tone(pin, 500, length);
	}
	
};



#endif

