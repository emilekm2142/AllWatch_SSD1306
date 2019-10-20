// Barometer.h

#ifndef _BAROMETER_h
#define _BAROMETER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Barometer
{
 protected:


 public:
	 virtual float getHeight() = 0;
	 virtual float getPressure() = 0;
	 virtual void sleep() = 0;
};



#endif

