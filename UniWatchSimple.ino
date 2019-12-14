
#include "StopwatchApp.h"
#include "AlarmApp.h"
#include "Buzzer.h"
#include "TestingEnv.h"
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
#include "StopwatchApp.h"
#include "FS.h"
#include "Buzzer.h"
#include "Config.h"
#include "GenericLoadingScreen.h"
#include "AlarmApp.h"
#ifdef RUN_TESTS
#include "AUnit.h"
#include "TestingEnv.h"
//#include "tests.h"


using namespace aunit;
#endif
GenericLoadingScreen loadingScreen = GenericLoadingScreen(&UserInterface);
ADC_MODE(ADC_VCC);
auto r = SSD1306Renderer();
auto bm = BatteryManager(&UserInterface);

extern "C" {
#include "user_interface.h"
}
void light_sleep() {
	wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
	wifi_fpm_open();
	wifi_enable_gpio_wakeup(12, GPIO_PIN_INTR_LOLEVEL);
	wifi_fpm_do_sleep(0xFFFFFFF);
	wifi_fpm_close();
}

Buzzer buzzer = Buzzer(BUZZER_PIN);

void onOk() {
	buzzer.PlayLong();
	UserInterface.Ok();
	UserInterface.StageChanges();
	bm.RegisterActivity();
}
void onUp() {
	buzzer.PlayShort();
	UserInterface.Up();
	bm.RegisterActivity();
	UserInterface.StageChanges();
}
void onDown() {
	buzzer.PlayShort();
	Serial.print("down");
	bm.RegisterActivity();
	UserInterface.Down();
	UserInterface.StageChanges();
}
void onBack() {
	buzzer.PlayLong();
	bm.RegisterActivity();
	UserInterface.StageChanges();
	UserInterface.Back();
}

InputHandler inputHandler = InputHandler(
	&onOk,
	&onUp,
	&onDown,
	&onBack,
	Serial
);


BME280Barometer barometerBME280 = BME280Barometer();
auto tk = TimeKepper(&UserInterface);

ExtraPeripheralsManager extraPeripheralsManager = ExtraPeripheralsManager(&barometerBME280, &tk, &buzzer);



auto MainLayout = MainSlideLayout();
auto settingsManager = SettingsManager(&WiFi, &SPIFFS, &tk, &extraPeripheralsManager);
auto home = HomeScreen(UserInterface, &tk, &settingsManager);
auto topBar = TopBar(&UserInterface, &bm, &tk, &settingsManager);
auto imageMenuScreen = ImagesMenu(&UserInterface);
//char* text = "mamamamam!!!$$@$@";
//auto genericText = GenericTextScreen(&UserInterface,text ,true);


//auto appsMenu = AppsMenu(&UserInterface, &settingsManager);



void setup() {
	bm.tk = &tk;
	bm.sm = &settingsManager;
#ifndef USE_TX_RX_AS_GPIO
	Serial.begin(115200);
	Serial.println("init...");
#endif // !USE_TX_RX_AS_GPIO
#ifdef APPEND_DEFAULT_WIFI
	if (settingsManager.wifiManager->IsNetworkSaved(settingsManager.SPIFFS, DEFAULT_WIFI_SSID))
	{
		settingsManager.wifiManager->RemoveWiFiNetwork(DEFAULT_WIFI_SSID);
	}
	settingsManager.wifiManager->AppendWiFiNetwork(settingsManager.SPIFFS, DEFAULT_WIFI_SSID, DEFAULT_WIFI_PASSWORD);
#endif // APPEND_DEFAULT_WIFI

	

	settingsManager.appsManager->RegisterApplication("Weather", []() {return new WeatherApp(&UserInterface, &settingsManager, &tk); }, WeatherApp_Icon::width, WeatherApp_Icon::height, WeatherApp_Icon::icon_bits);
	//settingsManager.appsManager->RegisterApplication("Stars", []() {return new StarsApp(&UserInterface, &settingsManager); }, StarsApp_Icon::width, StarsApp_Icon::height, StarsApp_Icon::icon_bits);

	settingsManager.appsManager->RegisterApplication("Flappy Bird", []() {return new FlappyBirdApp(&UserInterface, &settingsManager); }, FlappyBirdApp_Icon::width, FlappyBirdApp_Icon::height, FlappyBirdApp_Icon::icon_bits,false);
	settingsManager.appsManager->RegisterApplication("Games", []() {return new GamesApp(&UserInterface, &settingsManager); },GamesApp_Icon::width,GamesApp_Icon::height,GamesApp_Icon::icon_bits);
	settingsManager.appsManager->RegisterApplication("Stopwatch", []() {return new StopwatchApp(&UserInterface, &settingsManager); }, StopwatchApp_Icon::width,StopwatchApp_Icon::height, StopwatchApp_Icon::icon_bits);
	settingsManager.appsManager->RegisterApplication("IFTTT", []() {return new IFTTApp(&UserInterface, &settingsManager); }, IFTTApp_icon::width, IFTTApp_icon::height, IFTTApp_icon::iftt_bits);
	settingsManager.appsManager->RegisterApplication("Flashlight", []() {return new FlashlightApp(&UserInterface, &settingsManager); }, FlashlightApp_icon::width, FlashlightApp_icon::height, FlashlightApp_icon::icon_bits);
	settingsManager.appsManager->RegisterApplication("Set time", []() {return new TimeConfiguratorApp(&UserInterface, &settingsManager, &tk); }, TimeConfiguratorApp_Icon::width, TimeConfiguratorApp_Icon::height, TimeConfiguratorApp_Icon::icon_bits);
	settingsManager.appsManager->RegisterApplication("Status", []() {return new StatusApp(&UserInterface, &settingsManager); }, StatusApp_Icon::width, StatusApp_Icon::height, StatusApp_Icon::icon_bits);
	settingsManager.appsManager->RegisterApplication("Settings", []() {return new SettingsApp(&UserInterface, &settingsManager, &bm); }, SettingsApp_Icon::width, SettingsApp_Icon::height, SettingsApp_Icon::icon_bits);
	settingsManager.appsManager->RegisterApplication("WiFi", []() {return new WiFiConnectApp(&UserInterface, &settingsManager); }, WiFiConnectApp_Icon::width, WiFiConnectApp_Icon::height, WiFiConnectApp_Icon::icon_bits);
	settingsManager.appsManager->RegisterApplication("AppMarket", []() {return new AppMarketApp(&UserInterface, &settingsManager); }, AppMarketApp_Icon::width, AppMarketApp_Icon::height, AppMarketApp_Icon::icon_bits);
	settingsManager.appsManager->RegisterApplication("Alarm", []() {return new AlarmApp(&UserInterface, &settingsManager); }, AppMarketApp_Icon::width, AppMarketApp_Icon::height, AppMarketApp_Icon::icon_bits, false);

	//reading from spiffs
	bm.GetSleepTimeSeconds();

	
	if (!settingsManager.appsManager->KeyExists("IFTTT", "key")) {
		settingsManager.appsManager->AppendKeyToConfig("IFTTT", "key", "type in the api key");
	}
	for (int i = 0; i < settingsManager.appsManager->builtInApps->size(); i++) {
		auto app = settingsManager.appsManager->builtInApps->get(i);
		if (app->showOnAppsList)
		imageMenuScreen.AddOption(app->name, app->iconWidth, app->iconHeight, app->icon, [i]() {settingsManager.appsManager->builtInApps->get(i)->creatingFunction()->Open(); });


	}
	
	
	

	Serial.println("start");
	//Serial.println("UPDATED!!");

	delay(1000);


	r.init();

	UserInterface.SetRenderer(r);

	
	MainLayout.Add(&home);
	
//	MainLayout.Add(&commons);
//	MainLayout.Add(&appsMenu);
	MainLayout.Add((Layout*)&imageMenuScreen);
	//MainLayout.Add((Layout*)&genericText);
	MainLayout.UI = &UserInterface;
	
	
	UserInterface.SetMainLayout(MainLayout);

	UserInterface.AddSecondaryLayout(&topBar);
	UserInterface.RedrawAll();
	imageMenuScreen.setCurrentScroll();

	

#ifdef START_WITH_OTA_PORT
	settingsManager.w->begin(OTA_WIFI_SSID, OTA_WIFI_PASSWORD);
	settingsManager.SetupOTA();
#endif
	//settingsManager.TestSaveWiFi();
#ifdef RUN_TESTS
	TestingEnv::UI = &UserInterface;
	TestingEnv::sm = &settingsManager;
	if (settingsManager.wifiManager->IsNetworkSaved(settingsManager.SPIFFS, TESTS_SSID))
	{
		settingsManager.wifiManager->RemoveWiFiNetwork(TESTS_SSID);
	}
	settingsManager.wifiManager->AppendWiFiNetwork(settingsManager.SPIFFS, TESTS_SSID, TESTS_PASSWORD);
	delay(2000);
	TestRunner::setVerbosity(Verbosity::kAll);
	TestRunner::setTimeout(30);
#endif


	//Alarm Checking
	if (settingsManager.IsItTheTimeToTriggerAlarmOne())
	{
		if (settingsManager.appsManager->currentApplication != NULL)
			settingsManager.appsManager->currentApplication->Exit();
		settingsManager.appsManager->getBuiltInApplicationByName("Alarm")->getApplication()->Open();
	}

	

	
}
void loop() {
	
	inputHandler.OnLoop();
	UserInterface.OnLoop();
	tk.OnLoop();
	bm.OnLoop();
	Run::Loop();
	settingsManager.SettingsOnLoop();
	if (settingsManager.appsManager->currentApplication != NULL) {
		
		settingsManager.appsManager->currentApplication->OnLoop();
	}
#ifdef RUN_TESTS
	TestRunner::run();
#endif
}
