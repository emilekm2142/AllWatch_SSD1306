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
#include "GenericTextScreen.h"
#include "GenericInputScreen.h"
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
	class ForecastLayout :public CustomScreen {
	public:
		int offset = 15;
		char header[25];
		Layout* parent;
		WeatherApp* app;
		ForecastLayout(Layout*parent, WeatherApp* app) {
			this->app = app;
			this->parent = parent;
		}
		void Back(Renderer& r) override {
			this->app->Exit();
			//((CustomScreen*)this->parent)->ReturnToPreviousScreen();
		}
		void Draw(Renderer& r) {
			sprintf(header, "Weather in %s", this->app->cityName);
			r.DrawAlignedString(r.GetScreenWidth() / 2 + GlobalX, offset + GlobalY, header, r.GetScreenWidth(), r.Center);
			r.SetFont((uint8_t*)ArialMT_Plain_10);
			r.DrawAlignedString(r.GetScreenWidth() / 2 + 0 + GlobalX, 13 + offset + GlobalY, this->app->state, r.GetScreenWidth(), r.Center);
			char str[4];
			r.SetFont((uint8_t*)ArialMT_Plain_16);
			sprintf(str, "%d C", this->app->temperature);
			r.DrawAlignedString(r.GetScreenWidth() / 2 + GlobalX, 25 + offset + GlobalY, str, r.GetScreenWidth(), r.Center);
			r.SetFont((uint8_t*)ArialMT_Plain_10);
		}
	};
	class WeatherLayout :public CustomScreen {
	public:
		
		bool configured = true;
		WeatherApp* app;
		ForecastLayout* forecastLayout;
		GenericInputScreen* textInput;
		
		WeatherLayout(WeatherApp* app) {
			this->UI = app->UI;
			this->app = app;
			this->forecastLayout = new ForecastLayout((Layout*)this, app);
			this->textInput = new GenericInputScreen(this->app->UI, "City:");
		
			this->textInput->onOkCallback = [this] {
				this->app->UpdateKeyInConfig("city", textInput->GetText());
				auto sm = this->app->settingsManager;
				this->app->OnOpen();
				DisplayScreen((Layout*)this->forecastLayout);
			};
		}
		void Draw(Renderer& r)override {
			currentScreen->Draw(r);
		}
		void Back(Renderer& r) override{
			currentScreen->Back(r);
		}
		void Up(Renderer& r) override {
			currentScreen->Up(r);
		}
		void Down(Renderer& r) override {
			currentScreen->Down(r);
		}
		void Ok(Renderer& renderer) override {
			currentScreen->Ok(renderer);
		}
		void ShowInputScreen() {
			DisplayScreen(textInput);
			textInput->OpenKeyboard();
			this->textInput->GetKeyboard()->onClose = [this] {
				this->textInput->Ok(*UI->GetRenderer());
			};
		}
	};
 protected:
	 WeatherLayout* l;

 public:
	 int temperature=-999;
	 char state[10];

	 bool downloading = true;
	 TimeKepper* tk;
	 char cityName[25];
	 WeatherApp(UserInterfaceClass* UI, SettingsManager* sm, TimeKepper* tk): BuiltInApplication((Layout*)this->l, UI, sm){
		 l= new WeatherLayout(this);
		 this->layout = l;
		 this->tk = tk;
		 this->name = "Weather";
		 if (!KeyExists("city")) {
			 Serial.println("nie istnieje taki key");
			 ((WeatherLayout*)this->layout)->ShowInputScreen();
		 }
		 else {
			 ((WeatherLayout*)this->layout)->DisplayScreen(((WeatherLayout*)this->layout)->forecastLayout);
		 }
		 
		
	 }
	 bool isTommorowBeingDisplayed = false;

	 void OnOpen() override {
		 ESP.wdtDisable();
		 GetKeyValue("city", cityName);
		 Serial.println("opening weather, connected?");
		 Serial.println(KeyExists("city"));
		 if (KeyExists("city")) {			
			downloading = true;
			UI->RedrawAll();
			DisplayToday();
			if (layout == nullptr) Serial.println("Pointer is null");
		 }
		 else {
			 
		 }
		 ESP.wdtEnable(1000);
	
	 }

	 
	 void DisplayToday() {
		 char buffer[100];
		 GetKeyValue("city", cityName);
		
		 sprintf(buffer, "http://serwer1969419.home.pl/watchservice/weather.php?city=%s", cityName);
		 auto a = settingsManager->http->MakeGetRequest(buffer);
	 	 if (a==nullptr)
	 	 {
			// Exit();
			 Serial.println("closed!");
	 	 }
		 else {
			 auto s = a->getStream();
			 s.readBytesUntil('\n', state, 15);
			 auto l = s.readBytesUntil('\n', state, 15);
			 state[l] = '\0';
			 char xd[3];
			 s.readBytesUntil('\n', xd, 3);
			 temperature = atoi(xd);
			 settingsManager->appsManager->UpdateKeyInConfig("weather", "tmpCache", xd);
			 Serial.println("Request done!");
			 Serial.println(temperature);
			 Serial.println(state);
			 settingsManager->http->EndRequest(a, false);
			 downloading = false;
			 UI->RedrawAll();
		 }
		 
	 }

};


#endif

