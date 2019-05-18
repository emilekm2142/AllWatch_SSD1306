// BatteryManager.h

#ifndef _BATTERYMANAGER_h
#define _BATTERYMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "UserInterface.h"
#include "FS.h"
//class DependenciesHolder;
//#include "Dependency.h"

class BatteryManager//: public Dependency
{
 protected:
	 bool disableAll = true;


	 int deepSleepDelay=45e6;
	 int deepSleepIDLETrigger = 6000;
	 int batteryCheckDelay=10000;
	 int lastBatteryCheck = 0;
	 bool alwaysWakeMode = false;
	 bool isGoingToSleep = false;
	 int lastActivity = 0;

	 int batteryLevel=0;
public:
	// DependenciesHolder* dependencies;
	UserInterfaceClass* UI;
	 BatteryManager(UserInterfaceClass* UI) {
		 lastActivity = millis();
		 this->UI = UI;
		 lastBatteryCheck = millis();
		 batteryLevel = analogRead(A0);
	}

	 void RegisterActivity() {
		 lastActivity = millis();
	 }
	bool IsGoingToSleep() {
		 return isGoingToSleep;
	 }

	 void SetAlwaysWakeMode(bool v) {
		 alwaysWakeMode = v;
		 if (!v) RegisterActivity();
	 }
	int GetBatteryLevel() {
		 return batteryLevel;
	 }

	void OnLoop() {
		if (!disableAll) {
			if (!alwaysWakeMode)
				if (deepSleepIDLETrigger < millis() - lastActivity) {
					isGoingToSleep = true;
					UI->RedrawAll();
					ESP.deepSleep(deepSleepDelay, RFMode::RF_DISABLED);
				}

			if (batteryCheckDelay < millis() - lastBatteryCheck) {
				lastBatteryCheck = millis();
				batteryLevel = analogRead(A0);
			}
		}
	}
};



#endif

