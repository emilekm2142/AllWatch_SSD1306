// TimeKepper.h

#ifndef _TIMEKEPPER_h
#define _TIMEKEPPER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <RtcDS3231.h>
#include <Wire.h>
#include "UserInterface.h"
#include "BatteryManager.h"
//#include "DependenciesHolder.h"
class TimeKepper//:public Dependency
{
private:

 protected:
	 long lastCheck=0;
	 RtcDS3231<TwoWire>* Rtc;
	
 public:
	// DependenciesHolder* dependencies;
	 RtcDateTime now;
	 UserInterfaceClass* UI;
	 BatteryManager* bm;
	 TimeKepper(UserInterfaceClass* _UI, BatteryManager* bm) {
		 RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
		 this->UI = _UI;
		 this->bm = bm;
		 Rtc = new RtcDS3231<TwoWire>(Wire);
		 Wire.begin(0, 2); // due to limited pins, use pin 0 and 2 for SDA, SCL
		 Rtc->Begin();
		 now = Rtc->GetDateTime();
		 
		 
		 //if (now.Year()<2019)
		//	 Rtc->SetDateTime(compiled);
	 }
	
	 void SetDateTime(int year, int month, int day, int hour, int minute, int second) {
		 RtcDateTime n = RtcDateTime(year, month, day, hour, minute, second);
		 Rtc->SetDateTime(n);
	 }
	 void OnLoop() {
		 if (1000  <= millis() - lastCheck) {
			// Serial.println("Getting new time");
			 now = Rtc->GetDateTime();
			 lastCheck = millis();
			 this->UI->RedrawAll();
		 }
	 }
	
};



#endif

