#include "ImagesMenu.h"
#include "TimeConfiguratorApp.h"
#include "FlappyBirdApp.h"
#include "ESPert.h"
#include "IFTTApp.h"
#include "StatusApp.h"
#include "FlashlightApp.h"
#include "ExtraPeripheralsManager.h"
#include "BME280Barometer.h"
#include "Thermometer.h"
#include "Barometer.h"
#include "HttpClient.h"
#include "PizzaApp.h"
#include "SimpleRequestsApp.h"
#include "SSH1106Renderer.h"
#include "ConnectingScreen.h"
#include "WeatherApp.h"
#include "BuiltInApplication.h"
#include "GenericMenuScreen.h"
#include "GenericTextScreen.h"
#include "AppsMenu.h"
#include "Dependency.h"
#include "DependenciesHolder.h"
#include "BatteryManager.h"
#include "SettingsScreen.h"
#include "SettingsManager.h"
#include "CommonActionsScreen.h"
#include "StaticResources.h"
#include "TopBar.h"
#include "Animation.h"
#include "MainSlideLayout.h"
#include "HomeScreen.h"
#include "CustomScreen.h"
#include <Arduino.h>
#include "SimpleMenuRecord.h"
#include "Column.h"
#include "Label.h"
#include "SSD1306Renderer.h"
#include "Widget.h"
#include "Layout.h"
#include "InputHandler.h"
#include "TimeKepper.h"
#include "Renderer.h"
#include "UserInterface.h"
#include "InputHandler.h"
#include "AbstractInputInterpreter.h"
#include "AbstractInputHandler.h"
#include "Column.h"
#include "Label.h"
#include <ESP8266WiFi.h>
#include "HomeScreen.h"
#include "TimeKepper.h"
#include "MainSlideLayout.h"
#include "CommonActionsScreen.h"
#include "TopBar.h"
#include "FS.h"
#include "ImagesMenu.h"
#include "GenericTextScreen.h"
#include "PizzaApp.h"
#include "Config.h"
#include "FlappyBirdApp.h"
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
void onOk() {
	
	UserInterface.Ok();
	UserInterface.StageChanges();
	bm.RegisterActivity();
}
void onUp() {
	UserInterface.Up();
	bm.RegisterActivity();
	UserInterface.StageChanges();
}
void onDown() {
	Serial.print("down");
	bm.RegisterActivity();
	UserInterface.Down();
	UserInterface.StageChanges();
}
void onBack() {
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

Column menu;
BME280Barometer barometerBME280 = BME280Barometer();
ExtraPeripheralsManager extraPeripheralsManager = ExtraPeripheralsManager(&barometerBME280, &barometerBME280);
auto tk = TimeKepper(&UserInterface, &bm);
auto r = SSD1306Renderer();

auto home = HomeScreen(UserInterface, &tk);
auto MainLayout = MainSlideLayout();
auto settingsManager = SettingsManager(&WiFi, &SPIFFS, &tk, &extraPeripheralsManager);
auto topBar = TopBar(&UserInterface, &bm, &tk, &settingsManager);

auto commons = CommonActionsScreen(&UserInterface, &settingsManager);
auto testScreen = ImagesMenu(&UserInterface);
//char* text = "mamamamam!!!$$@$@";
//auto genericText = GenericTextScreen(&UserInterface,text ,true);


auto appsMenu = AppsMenu(&UserInterface, &settingsManager);



void setup() {
#ifndef USE_TX_RX_AS_GPIO
	Serial.begin(115200);
#endif // !USE_TX_RX_AS_GPIO
#ifdef APPEND_DEFAULT_WIFI
	settingsManager.wifiManager->AppendWiFiNetwork(settingsManager.SPIFFS, DEFAULT_WIFI_SSID, DEFAULT_WIFI_PASSWORD);
#endif // APPEND_DEFAULT_WIFI



	settingsManager.appsManager->RegisterApplication("Weather", []() {return new WeatherApp(&UserInterface, &settingsManager, &tk); });
	settingsManager.appsManager->RegisterApplication("Flappy Bird", []() {return new FlappyBirdApp(&UserInterface, &settingsManager); });
	//settingsManager.appsManager->RegisterApplication("Pizza", []() {return new PizzaApp(&UserInterface, &settingsManager); });
	settingsManager.appsManager->RegisterApplication("IFTTT", []() {return new IFTTApp(&UserInterface, &settingsManager); });
	settingsManager.appsManager->RegisterApplication("Flashlight", []() {return new FlashlightApp(&UserInterface, &settingsManager); }, false);
	settingsManager.appsManager->RegisterApplication("Set time", []() {return new TimeConfiguratorApp(&UserInterface, &settingsManager, &tk); });
	settingsManager.appsManager->RegisterApplication("Status", []() {return new StatusApp(&UserInterface, &settingsManager); }, false);
	if (!settingsManager.appsManager->KeyExists("IFTTT", "key")) {
		settingsManager.appsManager->AppendKeyToConfig("IFTTT", "key", "type in the api key");
	}
	for (int i = 0; i < settingsManager.appsManager->builtInApps->size(); i++) {
		auto app = settingsManager.appsManager->builtInApps->get(i);
		testScreen.AddOption(app->name, UIAssets::baseline_videogame_asset_black_18dp_bits, [i]() {settingsManager.appsManager->builtInApps->get(i)->creatingFunction()->Open(); });


	}

	
	pinMode(A0, INPUT);

	Serial.println("start");


	delay(1000);


	r.init();

	UserInterface.SetRenderer(r);

	
	MainLayout.Add(&home);
	
	MainLayout.Add(&commons);
	MainLayout.Add(&appsMenu);
	MainLayout.Add((Layout*)&testScreen);
	//MainLayout.Add((Layout*)&genericText);
	MainLayout.UI = &UserInterface;
	commons.UI = &UserInterface;
	
	UserInterface.SetMainLayout(MainLayout);

	UserInterface.AddSecondaryLayout(&topBar);
	UserInterface.RedrawAll();
	
	//settingsManager.TestSaveWiFi();
}
void loop() {
	inputHandler.OnLoop();
	UserInterface.OnLoop();
	tk.OnLoop();
	bm.OnLoop();
	settingsManager.SettingsOnLoop();
	if (settingsManager.appsManager->currentApplication != NULL) {
		
		settingsManager.appsManager->currentApplication->OnLoop();
	}
	
}
