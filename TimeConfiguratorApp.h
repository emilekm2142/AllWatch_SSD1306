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

		int blinkDelay = 2000;
		int blinkTime = 1000;
		int lastBlinkTime = 0;
		boolean blinkNow = false;


		TimeConfiguratorLayout(TimeConfiguratorApp* a) {
		//	localDay = app->tk->now.Day();
		//	localMonth = app->tk->now.Month();
		//	localHour = app->tk->now.Hour();
		//	localMinute = app->tk->now.Minute();

			localDay = 1;
			localMonth = 1;
			localHour = 1;
			localMinute = 1;

			app = a;
		}
		void Draw(Renderer& r) override {
			int spacing = 10;
			int offset = 15;
			char hourstring[10];
			char minutestring[10];
			char daystring[10];
			char monthstring[10];
			snprintf_P(hourstring,10,PSTR("%02u"), localHour);
			snprintf_P(minutestring,10,PSTR("%02u"), localMinute);
			snprintf_P(daystring,10,PSTR("%02u"), localDay);
			snprintf_P(monthstring,10,PSTR("%02u"), localMonth);
			if (currentObject == 0)r.DrawString(0, offset + 2, "-"); r.DrawString(5, offset + 2, "Hour: "); 
			if (editMode) {
				 if (currentObject==0 && !blinkNow || currentObject != 0)  r.DrawString(40, offset + 2, hourstring);
				}
			else {
				r.DrawString(40, offset + 2, hourstring);
			}
			
			if (currentObject == 1)r.DrawString(0, offset + 2+spacing, "-"); r.DrawString(5, offset + 2 + spacing, "Minute: ");
			if (editMode) {
				if (currentObject == 1 && !blinkNow || currentObject!=1)  r.DrawString(60, offset + 2 + spacing + 2, minutestring);
			}
			else {
				r.DrawString(60, offset + 2 + spacing + 2, minutestring);
			}
				
			if (currentObject == 2)r.DrawString(0, offset + 2+2*spacing, "-");  r.DrawString(5, offset + 2 + 2*spacing, "Month: "); 
			if (currentObject == 2 && !blinkNow || currentObject != 2){ r.DrawString(60, offset + 2 + 2 * spacing, monthstring);
		}
			else {
			r.DrawString(60, offset + 2 + 2 * spacing, monthstring);
			}
				
			if (currentObject == 3)r.DrawString(0, offset + 2+3*spacing, "-");  r.DrawString(5, offset + 2 + 3*spacing, "Day: ");
			if (currentObject == 3 && !blinkNow || currentObject != 3) {
				r.DrawString(60, offset + 2 + 3 * spacing, daystring);
			}
			else {
				r.DrawString(60, offset + 2 + 3 * spacing, daystring);
			}
				
		


		}
		void Down(Renderer& r) override {
			Serial.println(currentObject);
			if (!editMode) {
				currentObject++;
				if (currentObject > objectsCount)
					currentObject--;
			}
			else {
				switch (currentObject) {
				case 0: {

					localHour--;
					if (localHour < 0) localHour++;
					break;
				}
				case 1: {
					localMinute--;
					if (localMinute < 0) localMinute++;
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
						localDay = days[localMonth - 1];
					}
					break;
				}

				}
			}
			Draw(r);
		}
		
		
		void Up(Renderer& r) override {
			Serial.println(currentObject);
			if (!editMode) {
				currentObject--;
				if (currentObject < 0 )
					currentObject++;
			}
			else {
				switch (currentObject) {
				case 0: {
					localHour++;
					if (localHour > 24) localHour--;
					break;
				}
				case 1: {
					localMinute++;
					if (localMinute > 60) localMinute--;
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
			Draw(r);
		}
		void Back(Renderer& r) override {

			
			if (!editMode) {
				
				this->app->Exit();
			}
			editMode = false;
			Serial.printf("%02u.%02u  %02u:%02u", localDay, localMonth, localHour, localMinute);
		}
		void Ok(Renderer& r) override {
			if (!editMode) {
				lastBlinkTime = millis();
				blinkNow = true;
			}
			editMode = !editMode;
			if (currentObject==4) {
				app->tk->SetDateTime(2019, localMonth, localDay, localHour, localMinute, 0);
				app->Exit();
			}
		}

		void DrawActiveIndicator(Renderer& r) override {}

		void Loop(Renderer& r) {
			
			//Blinking
			if (blinkNow) {
				
				if (millis() - lastBlinkTime > blinkTime) {
					blinkNow = false;
					
					
				}
			}
			else {
				//Serial.printf("%d - %d > %d", millis(), lastBlinkTime, blinkDelay);
				if (millis() - lastBlinkTime > blinkDelay) {
					lastBlinkTime = millis();
					blinkNow = true;
					
				}
			}
			Draw(r);
		}

	};

public:
	TimeConfiguratorLayout* l = new TimeConfiguratorLayout(this);
	TimeKepper* tk;
	TimeConfiguratorApp(UserInterfaceClass* UI, SettingsManager* sm, TimeKepper* tk) : BuiltInApplication((Layout*)this->l, UI, sm) {
		Serial.println("constructor");
		this->layout = l;
		this->tk = tk;
		Serial.println("after constructor");
	}
	void OnOpen() override {


	}
	void OnExit() override {
		Serial.println("On exit... Time configurator app");
	}
	void Loop(Renderer& r) override {
		this->l->Loop(r);
	}
	
};


#endif

