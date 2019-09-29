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

class BatteryManager
{
 protected:
	 bool disableAll = false;

	 const int highBatteryPoint = 4200;
	 const int lowBatteryPoint = 3300;
	 const int deepSleepTime = 30*1000;
	 const int inactivitySleepDelay = 30000;
	 const int batteryCheckDelay=10000;
	 int lastBatteryCheck = 0;
	 bool alwaysWakeMode = false;
	 bool isGoingToSleep = false;
	 int lastActivity = 0;
public:
	UserInterfaceClass* UI;
	 BatteryManager(UserInterfaceClass* UI) {
		 lastActivity = millis();
		 this->UI = UI;
		 lastBatteryCheck = millis(); 
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
		return ESP.getVcc();
	 }
	int GetBatteryPercentage() {
		int current = GetBatteryLevel() - lowBatteryPoint;
		int max = highBatteryPoint - lowBatteryPoint;
		return (int)(((float)current / (float)max)*100.0);
	}

	bool ShouldUsePowerSaverMode(){
		return GetBatteryLevel() < lowBatteryPoint;
	}


	void OnLoop() {
		int currentTime = millis();
		// if there was no user activity for `inactivitySleepDelay`
		if (currentTime - lastActivity > inactivitySleepDelay) {
			ESP.deepSleep(deepSleepTime, RFMode::RF_DISABLED);
		}
		//If there was no activity at all in the current run
		if (lastActivity == 0) {
			if (currentTime > inactivitySleepDelay) { //TODO: Add a condition that makes the watch go to sleep only after the time just turned to next minute
				ESP.deepSleep(deepSleepTime, RFMode::RF_DISABLED);
			}
		}
		
	}
	
};
#endif

