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
	void Disable()
	{
		enabled = false;
	}
	void Enable()
	{
		enabled = true;
	}
	bool IsEnabled()
	{
		return enabled;
	}
	Buzzer(int pin)
	{
		pinMode(pin, OUTPUT);
		this->pin = pin;
		StopAll();
	}
	void PlayShort()
	{
		if (!enabled) return;
		tone(pin, 2500, 10);
	}
	void PlayLong()
	{
		if (!enabled) return;
		tone(pin, 2000, 50);
	}
	void Play(unsigned long length, int f=30)
	{
		if (!enabled) return;
		tone(pin, f, length);
	}
	void ForcePlay(unsigned long length, int f = 30)
	{
		
		tone(pin, f, length);
	}
	void StopAll()
	{
		if (!enabled) return;
		noTone(pin);
	}
};



#endif

