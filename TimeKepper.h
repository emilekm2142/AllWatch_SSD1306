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
#include "Thermometer.h"
//#include "DependenciesHolder.h"
class TimeKepper: public Thermometer //:public Dependency
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
		this->UI = _UI;
		
		Rtc = new RtcDS3231<TwoWire>(Wire);
		Rtc->Begin();
		Rtc->SetIsRunning(true);
		Rtc->Enable32kHzPin(false);
		Rtc->SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth);
	 	





	 	
		//Rtc->setDateTime(__DATE__, __TIME__);
		now = Rtc->GetDateTime();
		//Serial.printf("Time: %d", now.minute);
	 }
	void Sleep()
	 {
	
	 }
	float getTemperatureC() override
	 {
		return Rtc->GetTemperature().AsFloatDegC()-2.0f;
	 	
	 }
	void DeleteAlarmOne()
	 {
		const DS3231AlarmOne alarmOne = DS3231AlarmOne(50, 25, 61, 61, DS3231AlarmOneControl_MinutesSecondsMatch);
		Rtc->SetAlarmOne(alarmOne);
	 }
	void SetAlarmOne(int day, int hour, int minute, int mode=0)
	 {
		const DS3231AlarmOne alarmOne = DS3231AlarmOne(day, hour, minute, 00, DS3231AlarmOneControl_HoursMinutesSecondsMatch);
		Rtc->SetAlarmOne(alarmOne);


	 	
	 }
	
	void SetAlarmTwo(int day, int hour, int minute, int mode=0)
	 {
		const DS3231AlarmTwo alarmTwo = DS3231AlarmTwo(day, hour, minute, DS3231AlarmTwoControl_HoursMinutesMatch);
	
		Rtc->SetAlarmTwo(alarmTwo);
	 }
	 RtcDateTime GetCurrentTime()
	 {
		 return this->Rtc->GetDateTime();
	 }
	 void SetDateTime(int year, int month, int day, int hour, int minute, int second) {
		 Serial.printf("setting datetime: %d/%d/%d, %d:%d:%d \n", year, month, day, hour, minute, second);
		 RtcDateTime n = RtcDateTime(year, month, day, hour, minute, second);
		// Rtc->setDateTime(year, month, day, hour, minute, second);
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

