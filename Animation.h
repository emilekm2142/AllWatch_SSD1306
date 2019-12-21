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
	 int animLength = 0;
	 std::function<float(double)> customFunction;
	 int startValue;
	 bool realTime = true;
	 int frames = 0;
	 int currentFrame = 0;
	 std::function<void()> callback =NULL;
 public:
	static std::function<float(double)> GetBezier(float y0, float y1, float y2, float y3)
	 {
		 return [y1,y2,y3,y0](double t)
		 {
			 if (t >= 1) { return 1.0f; }
			 	//https://math.stackexchange.com/questions/2571471/understanding-of-cubic-b%C3%A9zier-curves-in-one-dimension
				 float val = pow(1 - t, 3)*y0 + 3 * t*pow(1 - t, 2)*y1 +3 * pow(t, 2)*(1 - t)*y2 + pow(t, 3)*y3;
;				// float y = (float) (pow(1 - t, 3)*0.0f + 3 * pow(1 - t, 2)*t*P1y + 3 * (1 - t)*pow(t, 2)*P2y + pow(t, 3)*1.0f);
			 	//float x = (float)(pow(1 - t, 3)*0.0f + 3 * pow(1 - t, 2)*t*P1x + 3 * (1 - t)*pow(t, 2)*P2x + pow(t, 3)*1.0f);
				return 1-val;
			 
			 
		 };
	 }
	 Animation(int& prop, int targetValue, int delay=1, int step=1, std::function<float(double)> customFunction=NULL, int length=0) {
		 startTime = millis();
		 this->prop = &prop;
		 this->step = step;
		 this->startValue = prop;
		 this->targetValue = targetValue;
		 this->delay = delay;
		 lastStep = startTime;
		 this->customFunction = customFunction;
		 this->animLength = length;
	}
	void SetCallback(std::function<void()> fun)
	{
		callback = fun;
	}
	void CallCallback()
	{
		if (callback != NULL){
		callback();
			}
	}
	 long GetInterval() {
		 return millis()-lastStep ;
	}
	bool ShouldStepBeTaken() {
		if (customFunction != NULL)
		{
			return true;
		}
		else {
			return  (int)GetInterval() >= (int)this->delay;
		}
	}
	void SetModeToFrameBased(int lengthInFrames)
	{
		frames = lengthInFrames;
		realTime = false;
	}
	void ExecuteStep(){
		//Serial.println(*prop);
		if (customFunction != NULL)
		{
			if (realTime) {
				*prop = startValue + (int)((float)(targetValue - startValue) * customFunction((double)(millis() - startTime) / (double)animLength));
				Serial.printf("Fun val: %f\n", customFunction((double)(millis() - startTime) / (double)animLength));
			}
			else
			{
				
				*prop = startValue + (int)((float)(targetValue - startValue) * customFunction((double)currentFrame/frames));
				Serial.printf("percentage: %f\n", (float)currentFrame / frames);
				currentFrame++;
			}
		}
		else {
			*prop = *prop + step;
		}
	}
	bool IsFinished() {
		bool a;
		if (customFunction != NULL)
		{
			if (!realTime)
			{
				return currentFrame > frames;
			}
			return (millis() - startTime) > animLength;
		}
		else {
			/*Serial.println("Target Value:");
			Serial.println(targetValue);
			Serial.println("Value:");
			Serial.println(*prop);
			Serial.println("----------");*/
			if (step > 0) {
				a = *prop >= targetValue;
			}
			else {
				a = *prop <= targetValue;
			}
			if (a) *prop = targetValue;
			return a;
		}
	}
	~Animation()
	{
		
	}
};


#endif

