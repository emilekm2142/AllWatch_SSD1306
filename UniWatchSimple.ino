#include "StarsApp.h"
#include "GenericDialogScreen.h"
#include "AppMarketApp.h"
#include "GamesApp.h"
#include "WiFiConnectApp.h"
#include "DelayedAction.h"
#include "SettingsApp.h"
#include "SSD1306Fonts.h"
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
//#include "AppsMenu.h"

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
#include "DelayedAction.h"
#include "FS.h"
#include "ImagesMenu.h"
#include "GenericTextScreen.h"
#include "PizzaApp.h"
#include "Config.h"
#include "StarsApp.h"
#include "FlappyBirdApp.h"
#include "SettingsApp.h"
#include "WiFiConnectApp.h"
#include "GamesApp.h"
#include "AppMarketApp.h"
ADC_MODE(ADC_VCC);

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
auto tk = TimeKepper(&UserInterface);
auto r = SSD1306Renderer();

auto home = HomeScreen(UserInterface, &tk);
auto MainLayout = MainSlideLayout();
auto settingsManager = SettingsManager(&WiFi, &SPIFFS, &tk, &extraPeripheralsManager);
auto topBar = TopBar(&UserInterface, &bm, &tk, &settingsManager);
auto imageMenuScreen = ImagesMenu(&UserInterface);
//char* text = "mamamamam!!!$$@$@";
//auto genericText = GenericTextScreen(&UserInterface,text ,true);


//auto appsMenu = AppsMenu(&UserInterface, &settingsManager);



void setup() {

	bm.sm = &settingsManager;
#ifndef USE_TX_RX_AS_GPIO
	Serial.begin(115200);
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
	settingsManager.appsManager->RegisterApplication("IFTTT", []() {return new IFTTApp(&UserInterface, &settingsManager); }, IFTTApp_icon::width, IFTTApp_icon::height, IFTTApp_icon::iftt_bits);
	settingsManager.appsManager->RegisterApplication("Flashlight", []() {return new FlashlightApp(&UserInterface, &settingsManager); }, FlashlightApp_icon::width, FlashlightApp_icon::height, FlashlightApp_icon::icon_bits);
	settingsManager.appsManager->RegisterApplication("Set time", []() {return new TimeConfiguratorApp(&UserInterface, &settingsManager, &tk); }, TimeConfiguratorApp_Icon::width, TimeConfiguratorApp_Icon::height, TimeConfiguratorApp_Icon::icon_bits);
	settingsManager.appsManager->RegisterApplication("Status", []() {return new StatusApp(&UserInterface, &settingsManager); }, StatusApp_Icon::width, StatusApp_Icon::height, StatusApp_Icon::icon_bits);
	settingsManager.appsManager->RegisterApplication("Settings", []() {return new SettingsApp(&UserInterface, &settingsManager); }, SettingsApp_Icon::width, SettingsApp_Icon::height, SettingsApp_Icon::icon_bits);
	settingsManager.appsManager->RegisterApplication("WiFi", []() {return new WiFiConnectApp(&UserInterface, &settingsManager); }, WiFiConnectApp_Icon::width, WiFiConnectApp_Icon::height, WiFiConnectApp_Icon::icon_bits);
	settingsManager.appsManager->RegisterApplication("AppMarket", []() {return new AppMarketApp(&UserInterface, &settingsManager); }, AppMarketApp_Icon::width, AppMarketApp_Icon::height, AppMarketApp_Icon::icon_bits);

	if (!settingsManager.appsManager->KeyExists("IFTTT", "key")) {
		settingsManager.appsManager->AppendKeyToConfig("IFTTT", "key", "type in the api key");
	}
	for (int i = 0; i < settingsManager.appsManager->builtInApps->size(); i++) {
		auto app = settingsManager.appsManager->builtInApps->get(i);
		if (app->showOnAppsList)
		imageMenuScreen.AddOption(app->name, app->iconWidth, app->iconHeight, app->icon, [i]() {settingsManager.appsManager->builtInApps->get(i)->creatingFunction()->Open(); });


	}
	
	
	pinMode(A0, INPUT);

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
	
}
