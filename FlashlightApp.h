// FlashlightApp.h

#ifndef _FLASHLIGHTAPP_h
#define _FLASHLIGHTAPP_h

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
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Layout.h"
class FlashlightApp: public BuiltInApplication
{
private:
	class FlashlightScreen :public CustomScreen {
	public:

		FlashlightApp* app;
		
		FlashlightScreen(FlashlightApp* app) {
			this->app = app;
			

		}
		void Draw(Renderer& r) override {
			r.FillRectangle(0, 0, r.GetScreenHeight(), r.GetScreenWidth());
		}
		void Back(Renderer& r) override {
			
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
	 FlashlightScreen* l;
	 FlashlightApp(UserInterfaceClass* UI, SettingsManager* sm) : BuiltInApplication((Layout*)this->l, UI, sm) {
		 this->l = new FlashlightScreen(this);
		 this->layout = l;
		 this->name = "Flashlight";
		 this->UI = UI;
		// this->UI->DisableDrawing();
		// this->layout->Draw(*UI->GetRenderer());
		 Serial.println("joy");

	 }
	 void OnExit() override {
		// this->UI->EnableDrawing();
	 }
};



#endif

