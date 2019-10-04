// StatusApp.h

#ifndef _STATUSAPP_h
#define _STATUSAPP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "BuiltInApplication.h"
#include "Renderer.h"
#include "CustomScreen.h"
#include "UserInterface.h"
#include "GenericMenuScreen.h"
#include  "Config.h"
#include "GenericTextScreen.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Layout.h"
namespace StatusApp_Icon {
	const int width = 36;
		const int height = 36;
	const unsigned char icon_bits[] PROGMEM = {
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0xc0, 0x3f, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x00,
	   0x00, 0x00, 0xfc, 0xff, 0x03, 0x00, 0x00, 0xfe, 0xff, 0x0f, 0x00, 0x80,
	   0xff, 0xff, 0x1f, 0x00, 0xc0, 0xff, 0xff, 0x3f, 0x00, 0xc0, 0xff, 0xff,
	   0x3f, 0x00, 0xe0, 0xff, 0xff, 0x7f, 0x00, 0xe0, 0xff, 0xf9, 0x7f, 0x00,
	   0xf0, 0xff, 0xf9, 0xff, 0x00, 0xf0, 0xff, 0xff, 0xff, 0x00, 0xf8, 0xff,
	   0xff, 0xff, 0x01, 0xf8, 0xff, 0xff, 0xff, 0x01, 0xf8, 0xff, 0xff, 0xff,
	   0x01, 0xf8, 0xff, 0xf9, 0xff, 0x01, 0xf8, 0xff, 0xf9, 0xff, 0x01, 0xf8,
	   0xff, 0xf9, 0xff, 0x01, 0xf8, 0xff, 0xf9, 0xff, 0x01, 0xf8, 0xff, 0xf9,
	   0xff, 0x01, 0xf0, 0xff, 0xf9, 0xff, 0x00, 0xf0, 0xff, 0xf9, 0xff, 0x00,
	   0xe0, 0xff, 0xf9, 0x7f, 0x00, 0xe0, 0xff, 0xff, 0x7f, 0x00, 0xc0, 0xff,
	   0xff, 0x3f, 0x00, 0xc0, 0xff, 0xff, 0x3f, 0x00, 0x80, 0xff, 0xff, 0x1f,
	   0x00, 0x00, 0xfe, 0xff, 0x0f, 0x00, 0x00, 0xfc, 0xff, 0x03, 0x00, 0x00,
	   0xf0, 0xff, 0x00, 0x00, 0x00, 0xc0, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

}
class StatusApp: public BuiltInApplication
{
private:
	class StatusAppScreen :public CustomScreen {
	public:

		StatusApp* app;
		GenericTextScreen* screen;
		char buffer[200];
		StatusAppScreen(StatusApp* app) {
			this->app = app;
			char* u = buffer;
			screen= new GenericTextScreen(app->UI, u, true);

		}
		void Draw(Renderer& r) override {
#ifdef TEMPERATURE_AVAILABLE
			snprintf_P(buffer,
				200,
				PSTR("My ip is: %s \n Temperature: %.1f*C \n Pressure: %.1f hPa \n Approx. altitude: %.2f"),
				this->app->settingsManager->w->localIP().toString().c_str(),
				this->app->settingsManager->extraPeripheralsManager->barometer->getPressure(),
				this->app->settingsManager->extraPeripheralsManager->thermometer->getTemperatureC(),
				this->app->settingsManager->extraPeripheralsManager->barometer->getHeight()
			);
#else
snprintf_P(buffer,
	200,
	PSTR("My ip is: %s \n"),
	this->app->settingsManager->w->localIP().toString().c_str()
	
);

#endif
			screen->Draw(r);
			
			
			
		}
		void Back(Renderer& r) override {
			//TODO: musi to byc? app->UI->ReturnToParentLayout();
			this->app->Exit();
		}
		void Down(Renderer& r) override {

		}
		void Up(Renderer& r) override {

		}
		void Ok(Renderer& r) override {

		}
	};


public:
	StatusAppScreen* l;
	StatusApp(UserInterfaceClass* UI, SettingsManager* sm) : BuiltInApplication((Layout*)this->l, UI, sm) {
		Serial.println("Welcome to the status app");
		this->l = new StatusAppScreen(this);
		this->layout = l;
		this->name = "Status";
		this->UI = UI;


	}
};

#endif

