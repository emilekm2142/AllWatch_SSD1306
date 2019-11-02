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
#include  "Config.h"
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

	 TimeKepper(UserInterfaceClass* _UI) {
		 RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
		 this->UI = _UI;
	
		 Rtc = new RtcDS3231<TwoWire>(Wire);
		 //Wire.begin(RTC_SDA, RTC_SCL); // due to limited pins, use pin 0 and 2 for SDA, SCL
		
		 Rtc->Begin();
		 Rtc->SetIsRunning(true);
		 now = Rtc->GetDateTime();
		 
		 //if (now.Year()<2019)
		//	 Rtc->SetDateTime(compiled);
	 }
	void Sleep()
	 {
		Rtc->SetIsRunning(false);
	 }
	 RtcDateTime GetCurrentTime()
	 {
		 return this->Rtc->GetDateTime();
	 }
	 void SetDateTime(int year, int month, int day, int hour, int minute, int second) {
		 Serial.printf("setting datetime: %d/%d/%d, %d:%d:%d \n", year, month, day, hour, minute, second);
		 RtcDateTime n = RtcDateTime(year, month, day, hour, minute, second);
		 Rtc->SetDateTime(n);
	 }
	 void OnLoop() {
	 	//TODO: usunac to z tego miejsca
		 if (1000  <= millis() - lastCheck) {
			// Serial.println("Getting new time");
			 now = Rtc->GetDateTime();
			 lastCheck = millis();
			 this->UI->RedrawAll();
		 }
	 }
	
};



#endif

