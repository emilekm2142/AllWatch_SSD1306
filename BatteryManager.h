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
#include "Config.h"

class BatteryManager
{
 protected:
#ifdef ENERGY_SAVING
	 bool disableAll = false;
#else
	 bool disableAll = true;
#endif
	 const int highBatteryPoint = 4400;
	 const int lowBatteryPoint = 3500;
	 const int deepSleepTime = 30*1000;
	 const int chargingPoint = 2965;
	 int inactivitySleepDelay = 30000;
	 const int batteryCheckDelay=10000;
	 unsigned long lastBatteryCheckTime = 0;
	 int lastBatteryCheck = 0;
	 bool alwaysWakeMode = false;
	 bool isGoingToSleep = false;
	 int lastActivity = 0;
public:
	SettingsManager* sm = nullptr;
	UserInterfaceClass* UI;
	TimeKepper* tk;
	 BatteryManager(UserInterfaceClass* UI) {
		 lastActivity = millis();
		 this->UI = UI;
		 lastBatteryCheckTime = millis();
		 lastBatteryCheck = __ReadBatteryLevel();
#ifdef SHUTDOWN_ON_LOW_VOLTAGE
	 	if (lastBatteryCheck < lowBatteryPoint)
		 {

			// ESP.deepSleep(0, RFMode::RF_DISABLED);
		 }
#endif
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
		return lastBatteryCheck;
	 }
	int __ReadBatteryLevel()
	 {
		lastBatteryCheckTime = millis();
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
		
		return res1;
	}

	bool ShouldUsePowerSaverMode(){
		return GetBatteryLevel() < lowBatteryPoint;
	}
	bool isBeingCharged()
	 {
		const int o = GetBatteryLevel();
		return o< chargingPoint + 3 && o > chargingPoint - 3;
	 }
	void Sleep()
	 {
		//tk->Sleep();
		UI->GetRenderer()->DisableScreen();
		sm->extraPeripheralsManager->barometer->sleep();
#ifdef WAKE_UP_FROM_SLEEP_AUTOMATICALLY
		ESP.deepSleep(GetSleepTimeSeconds()*1000, RFMode::RF_DISABLED);
#else
		ESP.deepSleep(ESP.deepSleepMax(), RFMode::RF_DISABLED);
#endif

	 }
	void SetSleepTimeSeconds(int time)
	 {
		fs::File a = sm->SPIFFS->open("/sleep", "w");
		inactivitySleepDelay = time*1000;
		a.write(time);
		a.close();
		
	 }
	int GetSleepTimeSeconds()
	 {
		if (sm->SPIFFS->exists("/sleep")) {
			fs::File a = sm->SPIFFS->open("/sleep", "r");
			uint8_t sec;
			inactivitySleepDelay = sec * 1000;
			a.read(&sec, sizeof(int8_t));
			a.close();
			return sec;
			
		}
		else
		{
			return (int)(deepSleepTime/1000);
		}
	 }
	
	void OnLoop() {
	 	if (millis() - lastBatteryCheckTime > 1000*45)
	 	{
			lastBatteryCheck = __ReadBatteryLevel();
#ifdef SHUTDOWN_ON_LOW_VOLTAGE
	 		if (lastBatteryCheck<lowBatteryPoint)
	 		{
				UI->GetRenderer()->DisableScreen();
				sm->extraPeripheralsManager->barometer->sleep();
			//	ESP.deepSleep(6e+8, RFMode::RF_DISABLED);
	 		}
#endif
	 	}
		if (disableAll) return;
		//Serial.println(ESP.getVcc());
	 	//sleep only if there is no application turned on
	 	if (sm->appsManager->currentApplication == nullptr && sm->appsManager->currentApplication == NULL)
	 	{
			int currentTime = millis();
			// if there was no user activity for `inactivitySleepDelay`
			if (currentTime - lastActivity > inactivitySleepDelay) {
				Sleep();
			}
			//If there was no activity at all in the current run
			if (lastActivity == 0) {
				if (currentTime > inactivitySleepDelay) { 
					Sleep();
				}
			}

		}
	 	}
		
	
};
#endif

