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
#include "GenericTextScreen.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Layout.h"
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
			snprintf_P(buffer,
				200,
				PSTR("My ip is: %s \n Temperature: %.1f*C \n Pressure: %.1f hPa \n Approx. altitude: %.2f"),
				this->app->settingsManager->w->localIP().toString().c_str(),
				this->app->settingsManager->extraPeripheralsManager->barometer->getPressure(),
				this->app->settingsManager->extraPeripheralsManager->thermometer->getTemperatureC(),
				this->app->settingsManager->extraPeripheralsManager->barometer->getHeight()
			);
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

