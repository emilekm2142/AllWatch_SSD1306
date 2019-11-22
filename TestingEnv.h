// TestingEnv.h

#ifndef _TESTINGENV_h
#define _TESTINGENV_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "GenericLoadingScreen.h"

#include "GenericDialogScreen.h"
#include "AppMarketApp.h"
#include "GamesApp.h"
#include "WiFiConnectApp.h"
#include "DelayedAction.h"
#include "SettingsApp.h"

#include "ImagesMenu.h"
#include "TimeConfiguratorApp.h"
#include "FlappyBirdApp.h"
#include "ESPert.h"
#include "IFTTApp.h"
#include "StatusApp.h"
#include "FlashlightApp.h"
#include "ExtraPeripheralsManager.h"
#include "BME280Barometer.h"
#include "WeatherApp.h"
#include "BuiltInApplication.h"

#include "BatteryManager.h"

#include "TopBar.h"

#include "MainSlideLayout.h"
#include "HomeScreen.h"

#include <Arduino.h>

#include "Column.h"

#include "SSD1306Renderer.h"

#include "Layout.h"
#include "InputHandler.h"
#include "TimeKepper.h"

#include "UserInterface.h"

#include <ESP8266WiFi.h>

#include "FS.h"

#include "Config.h"
#include "GenericLoadingScreen.h"
#include "SettingsManager.h"
namespace TestingEnv
{
	extern UserInterfaceClass* UI;
	extern SettingsManager* sm;
};
#endif

