#pragma once
// DosimeterApp.h

#ifndef _DOSIMETERAPP_h
#define _DOSIMETERAPP_h
#endif // 

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include "BuiltInApplication.h"
#include "Renderer.h"
#include "GenericMenuScreen.h"
#include "GenericTextScreen.h"
#include "CustomScreen.h"
#include "DelayedAction.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Layout.h"
#include "SSD1306.h"

namespace DosimeterApp_Icon {
	const int width = 36;
	const int height = 36;
	static unsigned char icon_bits[] PROGMEM = {
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x04, 0x00, 0x02, 0x00, 0x00, 0x07, 0x00, 0x0e, 0x00, 0x80,
   0x0f, 0x00, 0x1f, 0x00, 0xc0, 0x1f, 0x80, 0x3f, 0x00, 0xc0, 0x1f, 0x80,
   0x3f, 0x00, 0xe0, 0x3f, 0xc0, 0x7f, 0x00, 0xf0, 0x3f, 0xc0, 0x7f, 0x00,
   0xf0, 0x7f, 0xe0, 0xff, 0x00, 0xf0, 0x7f, 0xe0, 0xff, 0x00, 0xf8, 0x7f,
   0xe0, 0xff, 0x01, 0xf8, 0x3f, 0xcf, 0xff, 0x01, 0xf8, 0xbf, 0xdf, 0xff,
   0x01, 0xf8, 0x9f, 0xdf, 0xff, 0x01, 0x00, 0x80, 0x1f, 0x00, 0x00, 0x00,
   0x80, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x80, 0x1f, 0x00, 0x00, 0x00, 0x80, 0x1f, 0x00, 0x00,
   0x00, 0xc0, 0x3f, 0x00, 0x00, 0x00, 0xc0, 0x3f, 0x00, 0x00, 0x00, 0xe0,
   0x7f, 0x00, 0x00, 0x00, 0xe0, 0x7f, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x00,
   0x00, 0x00, 0xf8, 0xff, 0x01, 0x00, 0x00, 0xf8, 0xff, 0x01, 0x00, 0x00,
   0xf0, 0xff, 0x00, 0x00, 0x00, 0xc0, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

}
//TODO: This app has its own directory and own config management system. I should probably move it somewhere higher in the hierarchy later.
class DosimeterApp :public BuiltInApplication
{
private:
	class DosimeterAppLayout :public CustomScreen {
	public:
		DosimeterApp* app;

		bool editingNewNote = false;

		
		DosimeterAppLayout(DosimeterApp* app) {
			this->app = app;
		}

		void Draw(Renderer& r) {
			int d = -5;
			r.DrawAlignedString(r.GetHorizontalCenter(), r.GetScreenHeight() - 17 + d, "Adjust the radioactivity!",r.GetScreenWidth(), r.Center);

			r.DrawXBM(r.GetHorizontalCenter() - 18, r.GetVerticalCenter() - 18 + d, 36, 36, DosimeterApp_Icon::icon_bits);
			r.DrawRectangle(10, r.GetScreenHeight()-5 + d, 6, r.GetScreenWidth() - 20);
			r.FillRectangle(12, r.GetScreenHeight() - 3 + d, 2, ((float)app->range / 100.0f) * (float)(r.GetScreenWidth() - 24));
		}
		void Back(Renderer& r) override {
			this->app->Exit();
		}
		void DrawActiveIndicator(Renderer& renderer) override{

		}
		void Up(Renderer& renderer) override
		{
			app->range += 5;
			app->range = min(app->range, 100);
			
			app->UI->RedrawAll();
		}
		void Down(Renderer& renderer) override
		{
			app->range -= 5;
			app->range = max(app->range, 5);
		
			app->UI->RedrawAll();
		}
		void Ok(Renderer& r) override {
			currentScreen->Ok(r);
		}
	
	};
protected:
	DosimeterAppLayout* l = new DosimeterAppLayout(this);

public:

	ExtraPeripheralsManager* epm;
	DosimeterApp(UserInterfaceClass* UI, SettingsManager* sm, ExtraPeripheralsManager* epm) : BuiltInApplication((Layout*)this->l, UI, sm) {
		this->layout = l;
		this->name = (char*)PSTR("Notes");
		this->epm = epm;

	

	}
	


	void OnOpen() override {





	}
	void OnExit() override {

	}
	int range = 50;
	int middelay = 50;
	int maxFreq = 50;
	void Loop(Renderer& r) override {
		delay(random(middelay));

		int a = random(range);
		epm->buzzer->Play(20, a);
		delay(a);
	}
};
