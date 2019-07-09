// Thermometer.h

#ifndef _THERMOMETER_h
#define _THERMOMETER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Thermometer
{
 protected:


 public:
	 virtual int getTemperatureC() = 0;
	 
};


#endif

