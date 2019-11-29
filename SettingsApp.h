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
#include "Config.h"
#include "SSD1306.h"
#include "SettingsManager.h"
#include "GenericMenuScreen.h"
#include "GenericTextScreen.h"
#include "SSD1306Fonts.h"
#include <RtcDS3231.h>
#include "BatteryManager.h"
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
	class SleepTimeLayout :public CustomScreen
	{
	public:

		SettingsApp* app;
		CustomScreen* parent;
		int length;
		SleepTimeLayout(SettingsApp* app, CustomScreen* parent)
		{
			this->app = app; this->parent = parent;
			length=this->app->bm->GetSleepTimeSeconds();
		}
		void Draw(Renderer& r) override
		{
			auto strtemplate = PSTR("%02u s.");
			r.SetFont((uint8_t *)Orbitron_Medium_30);
			char bff[10];
			snprintf_P(bff, 10, strtemplate, length);

			r.DrawAlignedString(GlobalX + r.GetHorizontalCenter(), r.GetVerticalCenter() - 10, bff, r.GetScreenWidth(), r.Center);
			r.SetFont((uint8_t *)Orbitron_Medium_10);
		}
		void DrawActiveIndicator(Renderer& renderer) override
		{
			
		}
		void Down(Renderer& renderer) override
		{
			length--;
		}
		void Up(Renderer& renderer) override
		{
			length++;
		}
		void Ok(Renderer& renderer) override
		{
			this->app->bm->SetSleepTimeSeconds(length);
			Back(renderer);
		}
		void Back(Renderer& renderer) override
		{
			//Run::Cancel(blinkTask);
			this->parent->ReturnToPreviousScreen();
		}
	};
	class AlarmLayout:public CustomScreen
	{
	public:
		
		SettingsApp* app;
		CustomScreen* parent;
		int blinkCounter = 0;
		bool shouldBlink=false;
		int currentObject = 0;
		
		int localHour;
		int localMinute;

		Run::DelayedActionDataHolder* blinkTask;
		AlarmLayout(SettingsApp* app, CustomScreen* parent, int alarmNumber=0)
		{
			this->app = app;
			auto hour = app->settingsManager->tk->GetCurrentTime();
			localHour = hour.Hour();
			localMinute = hour.Minute();
			this->parent = parent;
			blinkTask = Run::Every(200,[this]()
			{
				shouldBlink = !shouldBlink;
				this->parent->DrawCurrentScreen(*this->app->UI->GetRenderer());
			});
			
		}
		void Draw(Renderer& r) override
		{
			auto strtemplate = PSTR("%02u");
			r.SetFont((uint8_t *)Orbitron_Medium_30);
			char hourBuff[10];
			snprintf_P(hourBuff, 10, strtemplate, localHour );
			char minuteBuff[10];
			snprintf_P(minuteBuff, 10, strtemplate, localMinute);

			r.DrawString(GlobalX + r.GetHorizontalCenter() - 45, r.GetVerticalCenter()-10, hourBuff);
			r.DrawString(GlobalX + r.GetHorizontalCenter() + 4, r.GetVerticalCenter()-10, minuteBuff);

			if (shouldBlink && currentObject==0) r.FillRectangle(GlobalX + r.GetHorizontalCenter() - 45, r.GetVerticalCenter() - 10, 26, 35,true);
			if (shouldBlink && currentObject==1) r.FillRectangle(GlobalX + r.GetHorizontalCenter() +4, r.GetVerticalCenter() - 10, 26, 35, true);
			r.SetFont((uint8_t *)Orbitron_Medium_10);
			r.DrawRectangle(GlobalX + r.GetHorizontalCenter() - 45/2, r.GetVerticalCenter() + 17, 13, 45);
			r.DrawAlignedString(GlobalX + r.GetHorizontalCenter() , r.GetVerticalCenter()+20, "ok",45, r.Center);
			
		}
		int GetSetHour()
		{
			return localHour;
		}
		int GetSetMinute()
		{
			return localMinute;
		}
		int GetSetSecond()
		{
			return 0;
		}
		void Back(Renderer& renderer) override
		{
			Run::Cancel(blinkTask);
			this->parent->ReturnToPreviousScreen();
		}
		void Down(Renderer& renderer) override
		{
			if (currentObject==0)
			{
				localHour--;
				if (localHour <= 0) localHour = 0;
			}
			else if (currentObject==1)
			{
				localMinute--;
				if (localMinute <= 0) localMinute = 0;
			}
		}
		void Up(Renderer& renderer) override
		{
			if (currentObject == 0)
			{
				localHour++;
				if (localHour >= 23) localHour = 23;
			}
			else if (currentObject == 1)
			{
				localMinute++;
				if (localMinute >= 60) localMinute = 60;
			}
		}
		void Ok(Renderer& renderer) override
		{
			if (currentObject == 0) currentObject = 1;
			if (currentObject == 1) currentObject = 2;
			if (currentObject == 2)
			{
				auto now = this->app->settingsManager->tk->GetCurrentTime();
				this->app->settingsManager->tk->SetAlarmOne(now.Day(), localHour, localMinute, DS3231AlarmOneControl_HoursMinutesSecondsMatch);
				//TODO:
				//Zapisywanie tego alarmu w pliku i wylaczanie czulosci na reset na 10 sekund przed nim jezeli zegarek chodzi! Nie da sie
				//Jezeli nie to przy kazdym resecie sprawdzanie czy jestesmy w minucie z alarmem
				Back(renderer);
			}
		}
		void DrawActiveIndicator(Renderer& renderer) override
		{
			
		}
	
		
	};
	class SettingsLayout:public CustomScreen {
	public:
		SettingsApp* app;
		GenericTextScreen* infoScreen;
		GenericMenuScreen* menu;

		AlarmLayout* alarmScreen;
		SleepTimeLayout* sleepTimeScreen;

		
		SettingsLayout(SettingsApp* app) {
		
			this->app = app;

			
			infoScreen = new GenericTextScreen(this->app->UI, (char*)F("."), true);
			menu =new GenericMenuScreen(app->UI);
			menu->AddOption((char*)F("Sync time"), [this]() {
				infoScreen->text = "Syncing...";
				DisplayLoadingScreenBlocking((Layout*)infoScreen, (Layout*)menu);
				if (this->app->settingsManager->SyncTime()>=0)
				{
					HideLoadingScreen();
				}
				else
				{
					infoScreen->text = "Could not connect to a network...";
					DisplayLoadingScreenBlocking((Layout*)infoScreen, (Layout*)menu);
					Run::After(1000,[this]
					{
						HideLoadingScreen();
					});
				}
				
			
			});
			menu->AddOption((char*)F("Set sleep timeout"), [this]() {

				sleepTimeScreen = new SleepTimeLayout(this->app, (CustomScreen*)this);
				DisplayScreen(sleepTimeScreen);


			});
			menu->AddOption((char*)F("Set alarm"), [this]() {

				alarmScreen = new AlarmLayout(this->app, (CustomScreen*)this);
				DisplayScreen(alarmScreen);
				

			});
			menu->AddOption((char*)F("Delete alarm"), [this]() {

				this->app->settingsManager->DeleteAlarmOne();


			});
			menu->AddOption((char*)F("Check for updates"), [this]() {
				infoScreen->text = "Getting updates...";
				DisplayScreen((Layout*)infoScreen);
				Draw(*this->app->UI->GetRenderer());
				Run::After(1000, [this]() {
					char urlBuffer[100];
					sprintf(urlBuffer, "http://watch-service-server.herokuapp.com/update/%d", WATCH_VERSION_NUMBER);
					Serial.println("EXECUTING REQUEST...");
					auto request =this->app->settingsManager->http->MakeGetRequest(urlBuffer);
					char rsp = request->getStream().read();
					Serial.println("RESULT READ:");
					Serial.println(rsp);
					Serial.println((int)rsp);
				
					if (rsp == '1') {
						this->app->settingsManager->InstallUpdate();
					}
					else if (rsp == '0') {
						const char* strPtr = PSTR("No update available");
						infoScreen->text = strPtr;
						Run::After(1000, [this]()
						{
							currentScreen= (Layout*)menu;
						});
					}
					this->app->settingsManager->http->EndRequest(request);
				});




			});
			menu->AddOption((char*)F("Own network"), [this]() {
				infoScreen->text = "Own Network........";
				this->app->settingsManager->CreateNetwork();
				this->app->settingsManager->OpenSettingsServer();
				infoScreen->text = "Navigate to 192.168.4.1";
				DisplayScreen((Layout*)infoScreen);
				Draw(*this->app->UI->GetRenderer());
			});
		
			menu->AddOption((char*)F("Existing network"), [this]() {
				//this->app->settingsManager->CloseNetwork();
				infoScreen->text = this->app->settingsManager->wifiManager->WiFiConnected()?(char*)"OK!":(char*)"You must connect to a network first";
				
				DisplayScreen((Layout*)infoScreen);
				Draw(*this->app->UI->GetRenderer());
				Run::After(1000, [this]() {
					DisplayScreen((Layout*)menu);
					if (this->app->settingsManager->wifiManager->WiFiConnected()) {
						auto o = new SettingsScreen(this->app->UI, this->app->settingsManager);
						DisplayScreen((Layout*)o);
						this->app->settingsManager->OpenSettingsServer();
					}
					Draw(*this->app->UI->GetRenderer());
				});
			});
#ifdef OTA_AVAILABLE
			menu->AddOption((char*)F("OTA Serial"), [this]() {
				this->app->settingsManager->wifiManager->ConnectToFirstFittingWiFiNetwork();
				delay(1000);
				infoScreen->text = this->app->settingsManager->wifiManager->WiFiConnected() ? (char*)"OK!" : (char*)"You must connect to a network first";

				currentScreen = (Layout*)infoScreen;
				Draw(*this->app->UI->GetRenderer());
				Run::After(1000, [this]() {
					currentScreen = (Layout*)menu;
					if (this->app->settingsManager->wifiManager->WiFiConnected()) {
						
						this->app->settingsManager->SetupOTA();
					}
					Draw(*this->app->UI->GetRenderer());
				});
			});
#endif
			
			currentScreen = (Layout*)menu;
		}
		void Draw(Renderer& r) {
			currentScreen->Draw(r);
		}
		void Back(Renderer& r) override {
			if (currentScreen==this)
				this->app->Exit();
			else
			{
				currentScreen->Back(r);
			}
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
	 BatteryManager* bm;

	 SettingsApp(UserInterfaceClass* UI, SettingsManager* sm, BatteryManager* bm) : BuiltInApplication((Layout*)this->l, UI, sm) {
		 this->layout = l;
		 this->name = "Settings";
		 this->bm = bm;

	 }

	 
	 void OnOpen() override {


	 }
	 void OnExit() override {
		 settingsManager->CloseNetwork();
		 settingsManager->CloseSettingsServer();
	 }


};


#endif

