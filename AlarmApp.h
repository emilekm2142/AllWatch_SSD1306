// AlarmApp.h

#ifndef _ALARMAPP_h
#define _ALARMAPP_h

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
#include <RtcDS3231.h>
#include "BatteryManager.h"
class AlarmApp:public BuiltInApplication
{
private:
	class AlarmScreenLayout:public CustomScreen
	{
	public:
		AlarmApp* app;
		AlarmScreenLayout(AlarmApp* app)
		{
			this->app = app; 
		
		}
		void Draw(Renderer& r) override
		{
			r.DrawAlignedString(r.GetHorizontalCenter(), r.GetVerticalCenter(), "Alarm!", r.GetScreenWidth(),r.Center);
		}
		void DrawActiveIndicator(Renderer& renderer) override
		{

		}
		void Up(Renderer& r) override {
			
		}
		void Down(Renderer& r) override {
			
		}
		void Ok(Renderer& renderer) override
		{
			Back(renderer);
		}
		void Back(Renderer& renderer) override
		{
			this->app->Exit();
		}
	};

public:

	AlarmScreenLayout* l = new AlarmScreenLayout(this);
	AlarmApp(UserInterfaceClass* UI, SettingsManager* sm) : BuiltInApplication((Layout*)this->l, UI, sm) {
		this->layout = this->l;
		


	}


	void OnOpen() override {
		this->settingsManager->extraPeripheralsManager->buzzer->Play(50000);

	}
	void OnExit() override {
		this->settingsManager->extraPeripheralsManager->buzzer->StopAll();

	
	}
};


#endif

