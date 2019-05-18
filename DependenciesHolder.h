// DependenciesHolder.h

#ifndef _DEPENDENCIESHOLDER_h
#define _DEPENDENCIESHOLDER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "UserInterface.h"
#include "BatteryManager.h"
#include "InputHandler.h"
#include "TimeKepper.h"
#include "SettingsManager.h"

//A bit unsafe to use. There is a moment just at the initialization, where the pointers can be null;
//DO NOT RELY ON THESE REFERENCES IN THE CONSTRUCTORS!
class DependenciesHolder{
 protected:


 public:
	/* UserInterfaceClass* UI;
	 BatteryManager* batteryManager;
	 InputHandler* inputHandler;
	 TimeKepper* timeKepper;
	 SettingsManager* settingsManager;
	 void ApplyDependencies();*/

};


#endif

