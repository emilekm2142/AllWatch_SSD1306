// TimeKepper.h

#ifndef _TIMEKEPPER_h
#define _TIMEKEPPER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <DS3231.h>
#include <Wire.h>
#include "UserInterface.h"
#include  "Config.h"
//#include "DependenciesHolder.h"
class TimeKepper//:public Dependency
{
private:

 protected:
	 long lastCheck=0;
	 DS3231* Rtc;
	
 public:
	// DependenciesHolder* dependencies;
	 RTCDateTime now;
	 UserInterfaceClass* UI;

	 TimeKepper(UserInterfaceClass* _UI) {
		this->UI = _UI;
		Wire.begin(0, 2);
		Rtc = new DS3231();
		Rtc->begin();
		// Wylaczamy wyjscie  32kHz
		Rtc->enable32kHz(false);

		// Ustawiamy wyjscie SQW na sygnal 1Hz
		Rtc->setOutput(DS3231_1HZ);

		// Wlaczamy wyjsscie SQW
		Rtc->enableOutput(true);
		Rtc->setDateTime(__DATE__, __TIME__);
		now = Rtc->getDateTime();
		Serial.printf("Time: %d", now.minute);
	 }
	void Sleep()
	 {
	
	 }
	void DeleteAlarmOne()
	 {
	 		Rtc->clearAlarm1();
	 }
	void SetAlarmOne(int day, int hour, int minute, int mode=0)
	 {
		Rtc->setAlarm1(day,hour,minute,0, DS3231_MATCH_H_M_S);



	 	
	 }
	
	void SetAlarmTwo(int day, int hour, int minute, int mode=0)
	 {
		Rtc->setAlarm2(day, hour, minute,DS3231_alarm2_t::DS3231_MATCH_H_M);
	 }
	 RTCDateTime GetCurrentTime()
	 {
		 return Rtc->getDateTime();
	 }
	 void SetDateTime(int year, int month, int day, int hour, int minute, int second) {
		 Serial.printf("setting datetime: %d/%d/%d, %d:%d:%d \n", year, month, day, hour, minute, second);
		 Rtc->setDateTime(year, month, day, hour, minute, second);
	 }
	 void OnLoop() {
	 	//TODO: usunac to z tego miejsca
		 if (1000  <= millis() - lastCheck) {
			// Serial.println("Getting new time");
			 now = Rtc->getDateTime();
			 lastCheck = millis();
			 this->UI->RedrawAll();
		 }
	 }
	
};



#endif

