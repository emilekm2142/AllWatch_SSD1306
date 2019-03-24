// TimeKepper.h

#ifndef _TIMEKEPPER_h
#define _TIMEKEPPER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class TimeKepper
{
private:
	int hour;
	int minute;
	int second;
	void RewalidateTime();
	

 protected:


 public:
	 int GetHour();
	 int GetMinute();
	 int GetSecond();
	 void(*OnDayEnd)();
	 void IncrementSecond();
	 int SetTime(int hour, int minute, int second);
};



#endif

