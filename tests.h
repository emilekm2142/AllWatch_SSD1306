// tests.h

#ifndef _TESTS_h
#define _TESTS_h
#include "Config.h"
#ifdef RUN_TESTS

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <AUnit.h>
#include "TestingEnv.h"
using namespace aunit;
using namespace TestingEnv;
void ResetToMainScreen()
{
	((MainSlideLayout*)TestingEnv::UI->GetMainLayout())->currentIndex = 0;
	if (sm->appsManager->currentApplication!=NULL)
		sm->appsManager->currentApplication->Exit();
	TestingEnv::UI->SetLayoutInFocues(*TestingEnv::UI->GetMainLayout());
}
void ResetNetworksList()
{
	sm->wifiManager->ClearList();
}

test(testInitializedCorrectly)
{
	//assertEqual(sm->wifiManager->IsNetworkSaved(sm->SPIFFS, "AUTO_TESTS"), true);
	assertNotEqual(TestingEnv::UI, NULL);
}

test(screenToRight)
{
	ResetToMainScreen();
	TestingEnv::UI->Down();
	delay(100);
	assertEqual(((MainSlideLayout*)TestingEnv::UI->GetMainLayout())->currentIndex,1);
	delay(1000);
}
test(appendNewNetwork)
{
	ResetNetworksList();
	ResetToMainScreen();
	
	sm->wifiManager->AppendWiFiNetwork(sm->SPIFFS, "test51", "test123");
	delay(100);
	assertTrue(sm->wifiManager->IsNetworkSaved(sm->SPIFFS,"test51"));
}

test(SyncHourGUI)
{
	delay(1000);
	ResetToMainScreen();
	SettingsApp* app = ((SettingsApp*)sm->appsManager->getBuiltInApplicationByName("Settings")->creatingFunction());
	app->Open();
	delay(200);
	UI->Ok();
	assertTrue(sm->wifiManager->WiFiConnected());
	
}
test(SyncHourCode)
{
	assertEqual(sm->SyncTime(),0);
}
test(ShowWeather)
{
	delay(1000);
	ResetToMainScreen();
	WeatherApp* app = ((WeatherApp*)sm->appsManager->getBuiltInApplicationByName("Weather")->creatingFunction());
	app->Open();
	while (sm->appsManager->currentApplication == app && app->downloading)
	{
		delay(100);
	}
	assertNotEqual(sm->appsManager->currentApplication, NULL);
	assertEqual(sm->appsManager->currentApplication, app);
	assertNotEqual(app->temperature, -999);
	
}
test(FlappyBird)
{
	delay(1000);
	ResetToMainScreen();
	//on moze sie tylko wykraszowac
	FlappyBirdApp* app = ((FlappyBirdApp*)sm->appsManager->getBuiltInApplicationByName("Flappy Bird")->creatingFunction());
	app->Open();
	UI->Ok();
	UI->Up();
	UI->Up();
	Run::After(8000, [app]()
	{
		app->Exit();
	});
	delay(8000);
}
test(AlarmOne)
{
	sm->SaveAlarmOne(12, 12, 12, false);
	sm->tk->SetDateTime(2019, 11, 12, 12, 12, 01);
	assertTrue(sm->IsItTheTimeToTriggerAlarmOne());
	sm->DeleteAlarmOne();
	assertFalse(sm->IsItTheTimeToTriggerAlarmOne());
}
test(ChangeHour)
{
	auto timeBefore = sm->tk->GetCurrentTime();
	sm->tk->SetDateTime(2019, 11, 11, 11, 11, 11);
	auto timeNow = sm->tk->GetCurrentTime();
	assertFalse(timeNow.Second() == timeBefore.Second() && timeNow.Minute() == timeBefore.Minute());
	sm->tk->SetDateTime(timeBefore.Year(), timeBefore.Month(), timeBefore.Day(), timeBefore.Hour(), timeBefore.Minute(), timeBefore.Second());
	
}
test(BarometerTest)
{
	assertMoreOrEqual(sm->extraPeripheralsManager->barometer->getPressure(), 1.0f);
}
test(CompassTest)
{
	
}
test(GyroTest)
{
	
}
test(IFTTTest)
{
	
}
test(AppConfigTests)
{

}
test(AlarmsTest)
{
	
}

#endif
#endif
