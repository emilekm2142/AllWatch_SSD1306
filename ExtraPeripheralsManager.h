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
#include "Buzzer.h"
class ExtraPeripheralsManager
{
 protected:


 public:
	 ExtraPeripheralsManager(Barometer* b, Thermometer* t, Buzzer* buzzer) { this->barometer = b; this->thermometer = t; this->buzzer = buzzer; }
	 Thermometer* thermometer;
	 Barometer* barometer;
	 Buzzer* buzzer;
};


#endif

