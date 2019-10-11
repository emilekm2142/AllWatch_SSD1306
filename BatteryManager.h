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
#include "SettingsManager.h"



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
	SettingsManager* sm = nullptr;
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
	int __GetBatteryPercentage() {
		int current = GetBatteryLevel() - lowBatteryPoint;
		int max = highBatteryPoint - lowBatteryPoint;
		int result = (int)(((float)current / (float)max)*100.0);
		return result > 100 ? 100 : result < 2 ? 2 : result;
	}
	int GetBatteryPercentage() {
		int res1 = __GetBatteryPercentage();
		int res2 = __GetBatteryPercentage();
		int res3 = __GetBatteryPercentage();
		return(min(res1, min(res2, res3)));
	}

	bool ShouldUsePowerSaverMode(){
		return GetBatteryLevel() < lowBatteryPoint;
	}

	
	void OnLoop() {
	 	//sleep only if there is no application turned on
	 	if (sm->appsManager->currentApplication == nullptr && sm->appsManager->currentApplication == NULL)
	 	{
			int currentTime = millis();
			// if there was no user activity for `inactivitySleepDelay`
			if (currentTime - lastActivity > inactivitySleepDelay) {
				//ESP.deepSleep(deepSleepTime, RFMode::RF_DISABLED);
			}
			//If there was no activity at all in the current run
			if (lastActivity == 0) {
				if (currentTime > inactivitySleepDelay) { //TODO: Add a condition that makes the watch go to sleep only after the time just turned to next minute
					//ESP.deepSleep(deepSleepTime, RFMode::RF_DISABLED);
				}
			}

		}
	 	}
		
	
};
#endif

