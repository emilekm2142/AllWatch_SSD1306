// PizzaApp.h

#ifndef _PIZZAAPP_h
#define _PIZZAAPP_h

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
#include "SSD1306.h"

//Potwierdzenie i tracking

class PizzaApp: public BuiltInApplication
{
private:
	class PizzaMainLayout :public CustomScreen {
	public:
		
		PizzaApp* app;
		GenericMenuScreen* mainMenu;
		PizzaMainLayout(PizzaApp* app) {
			this->app = app;
			mainMenu = new GenericMenuScreen(this->app->UI);
			mainMenu->AddOption("Margarita", [this]() {this->app->Exit(); });
			mainMenu->AddOption("Fungi", [this]() {this->app->Exit(); });
			mainMenu->AddOption("Vesuvio", [this]() {this->app->Exit(); });

		}
		void Draw(Renderer& r) override {
			mainMenu->Draw(r);
			r.DrawAlignedString(50, 25, "test!", 70, r.Right);
		}
		void Back(Renderer& r) override {
			app->UI->ReturnToParentLayout();
			this->app->Exit();
		}
		void Down(Renderer& r) override {
			Serial.println("Pizza main layout on down");
			mainMenu->Down(r);
		}
		void Up(Renderer& r) override {
			mainMenu->Up(r);
		}
		void Ok(Renderer& r) override {
			mainMenu->Ok(r);
		}
	};
protected:
	PizzaMainLayout* l; 


 public:
	 PizzaApp(UserInterfaceClass* UI, SettingsManager* sm) : BuiltInApplication((Layout*)this->l, UI, sm) {
		 this->l= new PizzaMainLayout(this);
		 this->layout = l;
		 this->name = "Pizza";
		 this->UI = UI;


	 }
	 void OnOpen() override {
		 Serial.println("Pizza app opened");
	 }
};



#endif

