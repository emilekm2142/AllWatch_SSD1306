// StopwatchApp.h

#ifndef _STOPWATCHAPP_h
#define _STOPWATCHAPP_h

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
#include "Config.h"
#include "SSD1306.h"
#include "SettingsManager.h"
#include "GenericMenuScreen.h"
#include "GenericTextScreen.h"
#include "SSD1306Fonts.h"

#include "BatteryManager.h"
namespace StopwatchApp_Icon {
	//source: https://www.flaticon.com/free-icon/stopwatch_149322?term=stopwatch&page=1&position=1
	const int width = 36;
	const int height = 36;
	static unsigned char icon_bits[]  PROGMEM = {
	   0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x40, 0x60, 0x00, 0x00, 0x00, 0x80,
   0x3b, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x01, 0x00, 0xc0, 0xff, 0x80,
   0x03, 0x00, 0x00, 0x84, 0xc3, 0x06, 0x00, 0x00, 0x04, 0x8e, 0x02, 0x00,
   0x00, 0x00, 0xda, 0x01, 0x00, 0x1c, 0x00, 0x70, 0x00, 0x00, 0x1c, 0x00,
   0x60, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00,
   0x00, 0x00, 0x00, 0xc0, 0x01, 0x80, 0x3f, 0x00, 0x02, 0x01, 0x00, 0x00,
   0x00, 0x03, 0x03, 0x00, 0x00, 0xc0, 0x01, 0x02, 0x00, 0x00, 0xe0, 0x00,
   0x02, 0xf0, 0x3f, 0xf0, 0x00, 0x02, 0x00, 0x00, 0x4c, 0x00, 0x06, 0x00,
   0x00, 0x26, 0x00, 0x07, 0x00, 0x00, 0x32, 0x00, 0x06, 0xfe, 0x3f, 0x1a,
   0x00, 0x06, 0x00, 0x00, 0x0c, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02,
   0x00, 0x00, 0x00, 0x00, 0x02, 0xe0, 0x3f, 0x00, 0x00, 0x03, 0x00, 0x00,
   0x00, 0x40, 0x01, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x80,
   0x00, 0x00, 0x1e, 0x00, 0x40, 0x00, 0x00, 0x1e, 0x00, 0x60, 0x00, 0x00,
   0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x04,
   0x07, 0x00, 0x00, 0x00, 0xe4, 0x01, 0x00, 0x00, 0xc0, 0x3f, 0x00, 0x00 };

}
class StopwatchApp:public BuiltInApplication
{
private:
	class StopwatchLayout :public CustomScreen
	{
	public:

		StopwatchApp* app;
		GenericMenuScreen* mid;
		int length;

		unsigned int startTime = 0;
		int second=0;
		unsigned diff = 0;
		int ms=0;

		char buffer[100][11];
		int counter = 0;

		bool displayScores = false;
		StopwatchLayout(StopwatchApp* app)
		{
			this->UI = app->UI;
			this->app = app;
			startTime = millis();
			mid = new GenericMenuScreen(UI);
		
		
		}
		void OnLoop(Renderer& r)
		{
			diff = millis() - startTime;
			second = diff / 1000;
			ms = diff % 1000;
			
			app->UI->RedrawAll();
		}
		void Ok(Renderer& renderer) override
		{
			
			sprintf(&buffer[counter][0], "%d. %d:%d", counter+1, second, ms);
			mid->AddOption(buffer[counter], [] {});
			counter++;
			if (counter > 100) counter = 0;
		}
		void Up(Renderer& renderer) override
		{
			if (displayScores) mid->Up(renderer);
			else displayScores = true;
			DisplayScreen((Layout*)mid);
		}
		void Down(Renderer& renderer) override
		{
			if (displayScores) mid->Down(renderer);
			else displayScores = true;
			DisplayScreen((Layout*)mid);
		}
		void Back(Renderer& renderer) override
		{
			if (!displayScores)
				app->Exit();
			else
				displayScores = false;
		}
		void Draw(Renderer& renderer) override
		{
			
			if (displayScores)
				DrawCurrentScreen(renderer);
			else
			{
				renderer.SetFont((uint8_t*)&Orbitron_Medium_30);
				char bufferS[3];
				char bufferMs[5];
				sprintf(&bufferS[0], "%02u", second);
				sprintf(&bufferMs[0], "%04u", ms);
				
				renderer.DrawAlignedString(renderer.GetHorizontalCenter(), renderer.GetVerticalCenter()-17, bufferS, renderer.GetScreenWidth(), renderer.Center);
				renderer.SetFont((uint8_t*)&Orbitron_Medium_10);
				renderer.DrawAlignedString(renderer.GetHorizontalCenter(), renderer.GetVerticalCenter()+15-6, bufferMs, renderer.GetScreenWidth(), renderer.Center);

				renderer.DrawAlignedString(renderer.GetHorizontalCenter(), renderer.GetScreenHeight()-11, "<OK> - Lap | laps >",renderer.GetScreenWidth(), renderer.Center);
			}
			renderer.DrawRectangle(0, renderer.GetScreenHeight() - 1, 1, ((float)ms / 1000.0f) * renderer.GetScreenWidth());
		}
		void DrawActiveIndicator(Renderer& renderer) override
		{
			
		}
	};
	protected:
		StopwatchLayout* l = new StopwatchLayout(this);

	public:


		StopwatchApp(UserInterfaceClass* UI, SettingsManager* sm) : BuiltInApplication((Layout*)this->l, UI, sm) {
			this->layout = l;
			this->UI = UI;
		}


		void OnOpen() override {


		}
		void OnExit() override {
			settingsManager->CloseNetwork();
			settingsManager->CloseSettingsServer();
		}
		void Loop(Renderer& r) override
		{
			l->OnLoop(r);
		}
};


#endif

