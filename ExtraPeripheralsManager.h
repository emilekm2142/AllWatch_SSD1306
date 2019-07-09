// ExtraPeripheralsManager.h

#ifndef _EXTRAPERIPHERALSMANAGER_h
#define _EXTRAPERIPHERALSMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Thermometer.h"
#include "Barometer.h"
class ExtraPeripheralsManager
{
 protected:


 public:
	 Thermometer* thermometer;
	 Barometer* barometer;
};


#endif

