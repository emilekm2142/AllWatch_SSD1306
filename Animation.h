// Animation.h

#ifndef _ANIMATION_h
#define _ANIMATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Animation
{
 protected:
	 long startTime;
	 long lastStep;
	 int* prop; int targetValue; int delay; int step;
 public:
	 Animation(int& prop, int targetValue, int delay, int step) {
		 startTime = millis();
		 this->prop = &prop;
		 this->step = step;
		
		 this->targetValue = targetValue;
		 this->delay = delay;
		 lastStep = startTime;
	}
	 long GetInterval() {
		 return millis()-lastStep ;
	}
	bool ShouldStepBeTaken() {
		return  (int)GetInterval() >= (int)this->delay;
	}
	void ExecuteStep(){
		//Serial.println(*prop);
		*prop =*prop + step;
	}
	bool IsFinished() {
		bool a;
		/*Serial.println("Target Value:");
		Serial.println(targetValue);
		Serial.println("Value:");
		Serial.println(*prop);
		Serial.println("----------");*/
		if (step > 0) {
			a= *prop >= targetValue;
		}
		else {
			a= *prop <= targetValue;
		}
		if (a) *prop = targetValue;
		return a;
	}
};


#endif

