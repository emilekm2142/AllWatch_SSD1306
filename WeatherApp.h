// WeatherApp.h

#ifndef _WEATHERAPP_h
#define _WEATHERAPP_h

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
#include "SSD1306.h"
struct WeatherAppConfig {
	char city[40];
	char* weatherServerIP;
};
namespace WeatherApp_Icon {
	const int width = 36;
	const int height = 36;
	const unsigned char icon_bits[] PROGMEM = {
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1f, 0x00, 0x00, 0x00,
	   0xe0, 0xff, 0x00, 0x00, 0x00, 0xf8, 0xff, 0x01, 0x00, 0x00, 0xfc, 0xff,
	   0x03, 0x00, 0x00, 0xfe, 0xff, 0x07, 0x00, 0x00, 0xff, 0xff, 0x0f, 0x00,
	   0xc0, 0xff, 0xff, 0x0f, 0x00, 0xf0, 0xff, 0xff, 0x1f, 0x00, 0xf8, 0xff,
	   0xff, 0x1f, 0x00, 0xfc, 0xff, 0xff, 0x7f, 0x00, 0xfe, 0xff, 0xff, 0xff,
	   0x01, 0xfe, 0xff, 0xff, 0xff, 0x03, 0xff, 0xff, 0xff, 0xff, 0x07, 0xff,
	   0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff,
	   0xff, 0x0f, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0xff, 0x0f,
	   0xfe, 0xff, 0xff, 0xff, 0x0f, 0xfe, 0xff, 0xff, 0xff, 0x07, 0xfc, 0xff,
	   0xff, 0xff, 0x07, 0xf8, 0xff, 0xff, 0xff, 0x03, 0xf0, 0xff, 0xff, 0xff,
	   0x01, 0xc0, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

}

class WeatherApp:public BuiltInApplication
{
private:
	class WeatherLayout :public CustomScreen {
	public:
		char header[25];
		bool configured = true;
		bool noData = false;
		WeatherApp* app;
		int offset = 15;
		WeatherLayout(WeatherApp* app) {
			this->app = app;
			
			
		}
		void Draw(Renderer& r) {
			//Serial.println("DRWAING!!!");
			//r.DrawXBM(25, 25, 32, 32, app->icon);
			if (this->app->downloading)
			{
				r.DrawAlignedString(r.GetScreenWidth() / 2 + GlobalX, offset + GlobalY, "Downloading...", r.GetScreenWidth(), r.Center);
			}
			else if (configured) {
				sprintf(header, "Weather in %s", this->app->config.city);
				r.DrawAlignedString(r.GetScreenWidth() / 2 + GlobalX, offset + GlobalY, header, r.GetScreenWidth(), r.Center);
				r.SetFont((uint8_t *)ArialMT_Plain_10);
				r.DrawAlignedString(r.GetScreenWidth() / 2 + 0 + GlobalX, 13 + offset + GlobalY, this->app->state, r.GetScreenWidth(), r.Center);
				char str[4];
				r.SetFont((uint8_t *)ArialMT_Plain_16);
				sprintf(str, "%d C", this->app->temperature);
				r.DrawAlignedString(r.GetScreenWidth() / 2 + GlobalX, 25 + offset + GlobalY, str, r.GetScreenWidth(), r.Center);
				r.SetFont((uint8_t *)ArialMT_Plain_10);
			}
			
			else if (!configured) {
				r.DrawAlignedString(r.GetScreenWidth() / 2 + GlobalX, offset + GlobalY, "Application is not configured, check settings!", r.GetScreenWidth(), r.Center);
			}
		}
		void Back(Renderer& r) override{
			//app->UI->ReturnToParentLayout();
			this->app->Exit();
		}
		void Up(Renderer& r) override {
			this->app->Up(r);
		}
		void Down(Renderer& r) override {
			this->app->Down(r);
		}
	};
 protected:
	 WeatherLayout* l = new WeatherLayout(this);

 public:
	 int temperature=-999;
	 char state[10];

	 bool downloading = true;
	 TimeKepper* tk;
	  struct WeatherAppConfig config = {"London","xd"};
	 
	 WeatherApp(UserInterfaceClass* UI, SettingsManager* sm, TimeKepper* tk): BuiltInApplication((Layout*)this->l, UI, sm){
		 this->layout = l;
		 this->tk = tk;
		 this->name = "Weather";
		 if (!KeyExists("city"))
			AppendKey("city", "Warszawa");
		 
		
	 }
	 bool isTommorowBeingDisplayed = false;
	 void Down(Renderer& r) {
		 if (!isTommorowBeingDisplayed) {
			 DisplayTommorow();
		 }
		 isTommorowBeingDisplayed = true;
		 l->Draw(r);
	 }
	 void Up(Renderer& r) {
		 if (isTommorowBeingDisplayed) {
			 DisplayToday();
		 }
		 isTommorowBeingDisplayed = false;
		 l->Draw(r);
	 }
	 void OnOpen() override {
		
		
		 Serial.println("opening weather, connected?");
		 Serial.println(KeyExists("city"));
		 if (KeyExists("city")) {			
			GetKeyValue("city", config.city);
			downloading = true;
			this->l->Draw(*UI->GetRenderer());
			DisplayToday();
			if (layout == nullptr) Serial.println("Pointer is null");
		 }
		 else {
			 l->configured = false;
		 }
		
		
	 }
	 void OnExit() override {
		 
	 }
	 
	 void DisplayToday() {
		 char buffer[100];
		 sprintf(buffer, "http://weather-watch-service.herokuapp.com/?city=%s", config.city);
		 auto a = settingsManager->http->MakeGetRequest(buffer);
	 	 if (a==nullptr)
	 	 {
			 Exit();
			 Serial.println("closed!");
	 	 }
		 else {
			 auto s = a->getStream();
			 s.readBytesUntil('\n', state, 15);
			 state[strlen(state)] = '\0';
			 char xd[3];
			 s.readBytesUntil('\n', xd, 3);
			 temperature = atoi(xd);
			 settingsManager->appsManager->UpdateKeyInConfig("weather", "tmpCache", xd);
			 Serial.println("Request done!");
			 Serial.println(temperature);
			 Serial.println(state);
			 settingsManager->http->EndRequest(a);
			 downloading = false;
			 UI->RedrawAll();
		 }
		 
	 }
	 void DisplayTommorow() {
		 //TODO: consolidate both functions
		 char buffer[100];
		 sprintf(buffer, "http://weather-watch-service.herokuapp.com/tommorow?city=%s", config.city);
		 auto a = settingsManager->http->MakeGetRequest(buffer);
		 auto s = a->getStream();
		 s.readBytesUntil('\n', state, 15);
		 state[strlen(state)] = '\0';
		 char xd[3];
		 s.readBytesUntil('\n', xd, 3);
		 temperature = atoi(xd);
		 settingsManager->appsManager->UpdateKeyInConfig("weather", "tmpCacheTommorow", xd);
		 Serial.println("Request done!");
		 Serial.println(temperature);
		 Serial.println(state);
		 settingsManager->http->EndRequest(a);
	 }
};


#endif

