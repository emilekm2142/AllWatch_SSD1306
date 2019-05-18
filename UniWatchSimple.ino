
/*
    Name:       UniWatchSimple.ino
    Created:	18-Mar-19 17:20:19
    Author:     DESKTOP-VQ942UG\Emile


	To port to another display, inherit from Renderer and write the implementations of virtual functions; then change the references in this file.
	To change the input mode, either modify existing inputHandler or write your own. It has to inherit from AbstractInputHandler.

	Requires:
	LinkedList by IvanSaidel- https://github.com/ivanseidel/LinkedList
	ESPAsyncWebServer: https://github.com/me-no-dev/ESPAsyncWebServer


*/


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
#include "GenericTextScreen.h"


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

auto tk = TimeKepper(&UserInterface, &bm);
auto r = SSD1306Renderer();

auto home = HomeScreen(UserInterface, &tk);
auto MainLayout = MainSlideLayout();
auto settingsManager = SettingsManager(&WiFi, &SPIFFS);
auto topBar = TopBar(&UserInterface, &bm, &tk, &settingsManager);

auto commons = CommonActionsScreen(&UserInterface, &settingsManager);

WeatherApp weather = WeatherApp(&UserInterface, &settingsManager, &tk);

char* text = "mamamamam!!!$$@$@";
auto genericText = GenericTextScreen(&UserInterface,text ,true);


auto appsMenu = AppsMenu(&UserInterface, &settingsManager);

void setup() {
	
	
	
	pinMode(5, INPUT_PULLUP);
	pinMode(4, INPUT_PULLUP);
	pinMode(A0, INPUT);
	

	Serial.begin(115200);
	Serial.println("start");


	delay(1000);


	r.init();

	UserInterface.SetRenderer(r);

	auto l1 = new Label("mama!");
	auto l2 = new Label("mama2");
	auto l3 = new Label("mama3");
	auto l4 = new Label("mama3");
	l2->canBeActivated = true;
	l2->onActivate = [] {Serial.println("option L2"); };
	
	l1->setBorder(true);
	l1->canBeActivated = true;
	l1->onActivate = [] {Serial.println("option L1"); };

	l3->canBeActivated = true;
	l3->onActivate = [] {Serial.print("asdasd"); };

	menu.Add(l1);
	menu.Add(l2);
	menu.Add(l3);

	menu.setBorder(true);
	menu.setY(20);
	MainLayout.Add(&home);
	
	MainLayout.Add(&commons);
	MainLayout.Add(&appsMenu);
	MainLayout.Add((Layout*)&genericText);
	MainLayout.UI = &UserInterface;
	commons.UI = &UserInterface;
	
	UserInterface.SetMainLayout(MainLayout);
	//UserInterface.ShowLayout(MainLayout);
	UserInterface.AddSecondaryLayout(&topBar);
	UserInterface.RedrawAll();
	//ESP.deepSleep(30e6, RFMode::RF_DISABLED);
	//UserInterface.ShowLayout(menu);
	//UserInterface.StageChanges();
	


	settingsManager.TestSaveWiFi();

}
//int freeRam() {
//	extern int __heap_start, *__brkval;
//	int v;
//	return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
//}
void loop() {
	//Serial.println();
	inputHandler.OnLoop();
	UserInterface.OnLoop();
	tk.OnLoop();
	bm.OnLoop();
	settingsManager.SettingsOnLoop();
	//strcpy(home.preganteText,String(analogRead(A0)).c_str());
	//UserInterface.StageChanges();
	//Serial.println(freeRam());
}