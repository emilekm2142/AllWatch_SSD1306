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
	void DeleteAlarmOne()
	 {
		const DS3231AlarmOne alarmOne = DS3231AlarmOne(50, 25, 61, 61, DS3231AlarmOneControl_MinutesSecondsMatch);
		Rtc->SetAlarmOne(alarmOne);
	 }
	void SetAlarmOne(int day, int hour, int minute, DS3231AlarmOneControl mode=DS3231AlarmOneControl_MinutesSecondsMatch)
	 {
	 	
		const DS3231AlarmOne alarmOne = DS3231AlarmOne(day,hour,minute, 00, mode);
		Rtc->SetAlarmOne(alarmOne);
	 }
	
	void SetAlarmTwo(int day, int hour, int minute, DS3231AlarmTwoControl mode)
	 {
		const DS3231AlarmTwo alarmTwo = DS3231AlarmTwo(day, hour, minute, mode);
		Rtc->SetAlarmTwo(alarmTwo);
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

