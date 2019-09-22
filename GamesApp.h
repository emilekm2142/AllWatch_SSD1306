// GamesApp.h

#ifndef _GAMESAPP_h
#define _GAMESAPP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "BuiltInApplication.h"
#include "Renderer.h"
#include "CustomScreen.h"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Layout.h"
#include "DelayedAction.h"
#include "SSD1306.h"
#include "SettingsManager.h"
#include "GenericMenuScreen.h"
#include "GenericTextScreen.h"
namespace GamesApp_Icon {

	const int width = 36;
	const int height = 36;
	const unsigned char icon_bits[] PROGMEM = {
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0xff,
	   0xff, 0x01, 0xfc, 0xff, 0xff, 0xff, 0x03, 0xfc, 0xff, 0xff, 0xff, 0x03,
	   0xfe, 0xf1, 0xff, 0xff, 0x07, 0xfe, 0xf1, 0xff, 0xff, 0x07, 0xfe, 0xf1,
	   0xff, 0x87, 0x07, 0xfe, 0xf1, 0xff, 0x87, 0x07, 0xfe, 0xf1, 0xff, 0x87,
	   0x07, 0x1e, 0x00, 0xff, 0xcf, 0x07, 0x1e, 0x00, 0x3f, 0xff, 0x07, 0xfe,
	   0xf1, 0x1f, 0xfe, 0x07, 0xfe, 0xf1, 0x1f, 0xfe, 0x07, 0xfe, 0xf1, 0x1f,
	   0xfe, 0x07, 0xfe, 0xf1, 0xff, 0xff, 0x07, 0xfe, 0xf1, 0xff, 0xff, 0x07,
	   0xfc, 0xff, 0xff, 0xff, 0x03, 0xfc, 0xff, 0xff, 0xff, 0x03, 0xf8, 0xff,
	   0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

}

class GamesApp :public BuiltInApplication
{
private:
	class GamesLayout :public CustomScreen {
	public:
		GamesApp* app;
		Layout* currentScreen; //TODO: move this to customScreen
		GenericMenuScreen* menu;
		GamesLayout(GamesApp* app) {
			this->app = app;


			menu = new GenericMenuScreen(app->UI);
			menu->AddOption((char*)F("Flappy Bird"), [this]() {
				this->app->Exit();
				this->app->settingsManager->appsManager->getBuiltInApplicationByName("Flappy Bird")->getApplication()->Open();

			});
			menu->AddOption((char*)F("More coming soon"), [this]() {
			

			});
		
			currentScreen = (Layout*)menu;

		}
		void Draw(Renderer& r) {
			currentScreen->Draw(r);
		}
		void Back(Renderer& r) override {
			this->app->Exit();
		}
		void Up(Renderer& r) override {
			currentScreen->Up(r);
		}
		void Down(Renderer& r) override {
			currentScreen->Down(r);
		}
		void Ok(Renderer& r) override {
			currentScreen->Ok(r);
		}
	};
protected:
	GamesLayout* l = new GamesLayout(this);

public:


	GamesApp(UserInterfaceClass* UI, SettingsManager* sm) : BuiltInApplication((Layout*)this->l, UI, sm) {
		this->layout = l;
		this->name = "Games";


	}


	void OnOpen() override {


	}
	void OnExit() override {
		
	}


};
#endif
