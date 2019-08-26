// TimeConfiguratorApp.h

#ifndef _TIMECONFIGURATORAPP_h
#define _TIMECONFIGURATORAPP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "BuiltInApplication.h"
#include "Renderer.h"
#include "CustomScreen.h"
#include <SSD1306Brzo.h> 
#include "UserInterface.h"
#include "TimeKepper.h"
class TimeConfiguratorApp : public BuiltInApplication
{
private:

	class TimeConfiguratorLayout :public CustomScreen {
	public:
		TimeConfiguratorApp* app;
		int days[12] = { 31,30,31,30,31,30,31,31,30,31,30,31 };
		int localHour;
		int localMinute;

		int localDay;
		int localMonth;

		boolean editMode = false;
		int currentObject = 0;
		int objectsCount = 4; //actually, +1 because we count with zero!

		int blinkDelay = 700;
		int blinkTime = 100;
		int lastBlinkTime = 0;
		boolean blinkNow = false;


		TimeConfiguratorLayout(TimeConfiguratorApp* a) {
			localDay = app->tk->now.Day();
			localMonth = app->tk->now.Month();
			localHour = app->tk->now.Hour();
			localMinute = app->tk->now.Minute();

			app = a;
		}
		void Draw(Renderer& r) override {
			int offset = 15;
			char timestring[20];
			char datestring[20];
			snprintf_P(datestring,
				20,
				blinkNow ?
				currentObject == 2 ?
				 PSTR("  .%02u") :
				currentObject == 3 ?
				 PSTR("%02u.  ") :
				PSTR("%02u.%02u")
				: PSTR("%02u.%02u"),
				localDay,
				localMonth

			);
			snprintf_P(timestring,
				20,
				blinkNow?
				 currentObject==0?
				  PSTR("  :%02u"):
				 currentObject==1?
			 	  PSTR("%02u:  "):
				 PSTR("%02u:%02u")
				:PSTR("%02u:%02u"),

				localHour,
				localMinute

			);
			r.SetFont((uint8_t *)ArialMT_Plain_10);
			r.DrawAlignedString(GlobalX + r.GetScreenWidth() / 2, GlobalY + 0 + offset, datestring, r.GetScreenWidth(), r.Center);
			r.SetFont((uint8_t *)ArialMT_Plain_24);
			r.DrawAlignedString(GlobalX + r.GetScreenWidth() / 2, GlobalY + 10 + offset, timestring, r.GetScreenWidth(), r.Center);
			r.SetFont((uint8_t *)ArialMT_Plain_10);
			r.DrawRectangle(GlobalX + r.GetScreenWidth() / 2 - 25, r.GetLineHeight() + GlobalY - 25, 15, 50);
			if (currentObject==4) r.DrawRectangle(GlobalX + r.GetScreenWidth() / 2 - 25 -1, r.GetScreenHeight() +GlobalY - 25-1, 17, 52);
			r.DrawAlignedString(GlobalX + r.GetScreenWidth() / 2, r.GetScreenHeight() + GlobalY + -20, (PROGMEM char*)("Ok"), r.GetScreenWidth(), r.Center);

		}
		void Up(Renderer& r) override {
			if (!editMode) {
				currentObject++;
				if (currentObject > objectsCount)
					currentObject--;
			}else{
				switch (currentObject) {
				case 0: {
					localHour++;
					break;
				}
				case 1: {
					localMinute++;
					break;
				}
				case 2: {
					localMonth++;
					if (localMonth > 12) {
						localMonth = 1;
					}
					break;
				}
				case 3: {
					localDay++;
					if (localDay > days[localMonth - 1]) {
						localDay = 1;
					}
					break;
				}
				}
			}
		}
		void Down(Renderer& r) override {
			if (!editMode) {
				currentObject--;
				if (currentObject < 0 )
					currentObject++;
			}
			else{
				switch (currentObject) {
				case 0: {
					localHour++;
					break;
				}
				case 1: {
					localMinute++;
					break;
				}
				case 2: {
					localMonth--;
					if (localMonth < 1) {
						localMonth = 12;
					}
					break;
				}
				case 3: {
					localDay--;
					if (localDay < 1) {
						localDay = days[localMonth-1];
					}
					break;
				}
				}
			}
		}
		void Back(Renderer& r) override {

			
			if (!editMode) {
				app->UI->ReturnToParentLayout();
				this->app->Exit();
			}
			editMode = false;
		}
		void Ok(Renderer& r) override {
			if (currentObject!=4)
			editMode = !editMode;
			else {
				app->tk->SetDateTime(2019, localMonth, localDay, localHour, localMinute, 0);
				app->Exit();
			}
			
		
		}
		void DrawActiveIndicator(Renderer& r) override {}

		void Loop() {
			//Blinking
			if (blinkNow) {
				if (millis() - lastBlinkTime > blinkTime) {
					blinkNow = false;
				}
			}
			else {
				if (millis() - lastBlinkTime > blinkDelay) {
					blinkNow = true;
				}
			}
		}

	};

public:
	TimeConfiguratorLayout* l = new TimeConfiguratorLayout(this);
	TimeKepper* tk;
	TimeConfiguratorApp(UserInterfaceClass* UI, SettingsManager* sm, TimeKepper* tk) : BuiltInApplication((Layout*)this->l, UI, sm) {
		this->layout = l;
		this->tk = tk;
	}
	void OnOpen() override {


	}
	void OnExit() override {
		delete l;
	}
	void Loop() {
		this->l->Loop();
	}
	
};


#endif

