// Dependency.h

#ifndef _DEPENDENCY_h
#define _DEPENDENCY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
class DependenciesHolder;
//#include "DependenciesHolder.h"
#include "UserInterface.h"
#include "BatteryManager.h"
#include "InputHandler.h"
#include "TimeKepper.h"
#include "SettingsManager.h"

class Dependency
{
public:
	//DependenciesHolder* dependencies;
};


#endif

