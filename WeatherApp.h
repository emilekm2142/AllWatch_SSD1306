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
			if (configured) {
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
			else if (noData || this->app->temperature==-999) {
				r.DrawAlignedString(r.GetScreenWidth() / 2 + GlobalX, offset + GlobalY, "No cache nor internet. Cannot display any weather", r.GetScreenWidth(), r.Center);
			}
			else if (!configured) {
				r.DrawAlignedString(r.GetScreenWidth() / 2 + GlobalX, offset + GlobalY, "Application is not configured, check settings!", r.GetScreenWidth(), r.Center);
			}
		}
		void Back(Renderer& r) override{
			app->UI->ReturnToParentLayout();
			this->app->Exit();
		}
	};
 protected:
	 WeatherLayout* l = new WeatherLayout(this);

 public:
	 int temperature=-999;
	 char state[10];


	 unsigned char icon[32 * 32] = {
		 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x02,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x80, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,
	  0x00, 0x20, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x04, 0x00, 0x80, 0x01, 0x06,
	  0x10, 0x00, 0x00, 0x02, 0x10, 0x00, 0x00, 0x03, 0x20, 0x00, 0x00, 0x01,
	  0x60, 0x00, 0x00, 0x01, 0xC0, 0x00, 0x80, 0x01, 0x80, 0x01, 0xC0, 0x00,
	  0x00, 0x07, 0x60, 0x00, 0x00, 0xFC, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
	 TimeKepper* tk;
	  struct WeatherAppConfig config = {"London","xd"};
	 
	 WeatherApp(UserInterfaceClass* UI, SettingsManager* sm, TimeKepper* tk): BuiltInApplication((Layout*)this->l, UI, sm){
		 this->layout = l;
		 this->tk = tk;
		 this->name = "weather";
		 if (!KeyExists("city"))
			AppendKey("city", "Warszawa");
		 
		
	 }
	 void OnOpen() override {
		

		 Serial.println("opening weather, connected?");
		 if (KeyExists("city")) {
			 
			
			
			 settingsManager->appsManager->GetKeyFromConfig("weather", "city", config.city);
			 
	

			 if (settingsManager->w->isConnected()) { DownloadData(); }
			 else {
				 if (settingsManager->appsManager->KeyExists("weather", "tmpCache")) {
					 char tmp[4];
					 settingsManager->appsManager->GetKeyFromConfig("weather", "tmpCache",tmp);
					 temperature = atoi(tmp);
				 }
				 else {
					 l->noData = true;
				 }
			 }
			 if (layout == nullptr) Serial.println("Pointer is null");
		 }
		 else {
			 l->configured = false;
		 }
		
		
	 }
	 void OnExit() override {
		 
	 }
	 void DownloadData() {
		 
		 

	
		 char buffer[100];
		 sprintf(buffer, "http://weather-watch-service.herokuapp.com/?city=%s", config.city);
		 auto a = settingsManager->http->MakeGetRequest(buffer);
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
		 
	 }

};


#endif

