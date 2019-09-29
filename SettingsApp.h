// SettingsApp.h

#ifndef _SETTINGSAPP_h
#define _SETTINGSAPP_h

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
namespace SettingsApp_Icon{
	const int width = 36;
	const int height = 36;
	static unsigned char icon_bits[]  PROGMEM = {
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x0f,
	   0x00, 0xe0, 0xff, 0xff, 0x7f, 0x00, 0xe0, 0xff, 0xff, 0x7f, 0x00, 0xe0,
	   0xff, 0xff, 0x7f, 0x00, 0xf0, 0x7f, 0xe0, 0xff, 0x00, 0xf0, 0x7f, 0xe0,
	   0xff, 0x00, 0xf0, 0x7f, 0xe0, 0xff, 0x00, 0xf0, 0x23, 0x40, 0xfc, 0x00,
	   0xf0, 0x01, 0x00, 0xf8, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x00, 0xf0, 0x00,
	   0x00, 0xf0, 0x00, 0xf0, 0x00, 0x0f, 0xf0, 0x00, 0xf0, 0x83, 0x1f, 0xfc,
	   0x00, 0xf0, 0x83, 0x1f, 0xfc, 0x00, 0xf0, 0x83, 0x1f, 0xfc, 0x00, 0xf0,
	   0x83, 0x1f, 0xfc, 0x00, 0xf0, 0x00, 0x0e, 0xf0, 0x00, 0xf0, 0x00, 0x00,
	   0xf0, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x00, 0xf0, 0x01, 0x00, 0xf8, 0x00,
	   0xf0, 0x23, 0x40, 0xfc, 0x00, 0xf0, 0x7f, 0xe0, 0xff, 0x00, 0xf0, 0x7f,
	   0xe0, 0xff, 0x00, 0xf0, 0x7f, 0xe0, 0xff, 0x00, 0xe0, 0xff, 0xff, 0x7f,
	   0x00, 0xe0, 0xff, 0xff, 0x7f, 0x00, 0xe0, 0xff, 0xff, 0x7f, 0x00, 0x00,
	   0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

}

class SettingsApp:public BuiltInApplication
{
private:
	class SettingsLayout:public CustomScreen {
	public:
		SettingsApp* app;
		Layout* currentScreen; //TODO: move this to customScreen
		GenericTextScreen* infoScreen;
		GenericMenuScreen* menu;
		SettingsLayout(SettingsApp* app) {
			this->app = app;

			
			infoScreen = new GenericTextScreen(this->app->UI, (char*)F("joy xdxd"), true);
			menu =new GenericMenuScreen(app->UI);
			menu->AddOption((char*)F("Sync time"), [this]() {
				infoScreen->text = "Syncing...";
				currentScreen = (Layout*)infoScreen;
				Draw(*this->app->UI->GetRenderer());
				Run::After(1000, [this]() {
					this->app->settingsManager->SyncTime();
					currentScreen = (Layout*)menu;
				});
			});
			menu->AddOption((char*)F("Check for updates"), [this]() {
				infoScreen->text = "Getting updates...";
				currentScreen = (Layout*)infoScreen;
				Draw(*this->app->UI->GetRenderer());
				Run::After(1000, [this]() {
					char urlBuffer[100];
					sprintf(urlBuffer, "http://watch-service-server.herokuapp.com/update/%d", 1);
					auto request =this->app->settingsManager->http->MakeGetRequest(urlBuffer);
					char rsp = request->getStream().read();
					Serial.println(rsp);
					if (rsp == '1') {

					}
					else if (rsp == '0') {
						infoScreen->text = "No update available"
					}
					this->app->settingsManager->http->EndRequest(request);
				});




			});
			menu->AddOption((char*)F("Own network"), [this]() {
				infoScreen->text = "Own Network........";
				this->app->settingsManager->CreateNetwork();
				this->app->settingsManager->OpenSettingsServer();
				infoScreen->text = "Navigate to 192.168.1.4";
				currentScreen = (Layout*)infoScreen;
				Draw(*this->app->UI->GetRenderer());
			});
		
			menu->AddOption((char*)F("Existing network"), [this]() {
				//this->app->settingsManager->CloseNetwork();
				infoScreen->text = this->app->settingsManager->wifiManager->WiFiConnected()?(char*)"OK!":(char*)"You must connect to a network first";
				
				currentScreen = (Layout*)infoScreen;
				Draw(*this->app->UI->GetRenderer());
				Run::After(1000, [this]() {
					currentScreen = (Layout*)menu;
					if (this->app->settingsManager->wifiManager->WiFiConnected()) {
						//TODO: does it work?
						auto o = SettingsScreen(this->app->UI, this->app->settingsManager);
						currentScreen = (Layout*)&o;
						this->app->settingsManager->OpenSettingsServer();
					}
					Draw(*this->app->UI->GetRenderer());
				});
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
	 SettingsLayout* l = new SettingsLayout(this);

 public:


	 SettingsApp(UserInterfaceClass* UI, SettingsManager* sm) : BuiltInApplication((Layout*)this->l, UI, sm) {
		 this->layout = l;
		 this->name = "Settings";


	 }

	 
	 void OnOpen() override {


	 }
	 void OnExit() override {
		 settingsManager->CloseNetwork();
		 settingsManager->CloseSettingsServer();
	 }


};


#endif

