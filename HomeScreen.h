// HomeScreen.h

#ifndef _HOMESCREEN_h
#define _HOMESCREEN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "CustomScreen.h"

#include "UserInterface.h"
#include "TimeKepper.h"
#include "SSD1306Fonts.h"
#include "SettingsManager.h"
class HomeScreen: public CustomScreen
{
 protected:
	 UserInterfaceClass* UI;
	 TimeKepper* tk;
	 SettingsManager* sm;
 public:
	 int currentSecondsOffset = 0;
	 int prevSecond = 0;
	 HomeScreen(UserInterfaceClass& ui, TimeKepper* tk, SettingsManager* sm) {
		 UI = &ui;
		 this->sm = sm;
		 this->tk = tk;
	 }
	 void Draw(Renderer& r) override {
		 int offset =6;
		 char timestring[20];
		 char datestring[20];
		 char* days[] = { "Su","Mon", "Tue", "Wed", "Thu", "Fr", "Sa" };
		 snprintf_P(datestring,
			 20,
			 PSTR("%s, %02u.%02u"),
			 days[tk->now.DayOfWeek()],
			 tk->now.Day(),
			 tk->now.Month()

		 );

		 snprintf_P(timestring,
			 20,
			 PSTR("%02u:%02u"),

			 tk->now.Hour(),
			 tk->now.Minute()
			 
		 );



		 r.SetFont((uint8_t *) Orbitron_Medium_10);
		 r.DrawAlignedString(0, 0, datestring, r.GetScreenWidth(), r.Left);
		 r.SetFont((uint8_t *)Orbitron_Medium_30);
		 r.DrawAlignedString(GlobalX+r.GetScreenWidth()/2 -7, GlobalY+13+offset, timestring, r.GetScreenWidth(), r.Center);
		 const int bigTextWidth = r.GetStringWidth(timestring);
		 r.SetFont((uint8_t *)Orbitron_Medium_10);
		
#ifdef DISPLAY_SECONDS
	 	
		 char secondBuffer[5];
		
		 snprintf_P(secondBuffer,
			 5,
			 PSTR("%02u"),
			 tk->now.Second()

		 );
		 r.DrawAlignedString(GlobalX + r.GetScreenWidth() / 2 - 7 + bigTextWidth/2 + 1, GlobalY + 13 + offset + 16 +currentSecondsOffset, secondBuffer , r.GetScreenWidth(), r.Left);
	 	if (prevSecond!=tk->now.Second())
	 	{
	 		
	 	}
		 prevSecond = tk->now.Second();
#endif
#if defined(HOME_SCREEN_TEMPERATURE_READINGS) && defined(TEMPERATURE_AVAILABLE) 
		 const float pressure = sm->extraPeripheralsManager->barometer->getPressure();
		 const float temperature = sm->extraPeripheralsManager->thermometer->getTemperatureC();
		 char tmpBuffer[15];
		 snprintf_P(tmpBuffer,
			 15,
			 PSTR(" %.1f*C"),
			 temperature
		 );
		 char prsBuffer[15];
		 snprintf_P(prsBuffer,
			 15,
			 PSTR(" %.1fhPa"),
			 pressure
		 );
		 r.DrawString(GlobalX, GlobalY+r.GetScreenHeight() - 12, tmpBuffer);
		 r.DrawString(GlobalX+r.GetScreenWidth()-65, GlobalY+r.GetScreenHeight() - 12, prsBuffer);
		 r.DrawRectangle(GlobalX, GlobalY + r.GetScreenHeight() - 15, 1, r.GetScreenWidth());
#endif
	 }
	 virtual void Up(Renderer& renderer) override {

	 }
	 virtual void Down(Renderer& renderer) override {

	 }
	 virtual void Ok(Renderer& renderer) override {

	 }
	 virtual void Back(Renderer& renderer) override {

	 }
	 virtual void OnGetInFocus(Renderer& renderer) override {
		 UI->SetLayoutInFocues(*UI->GetMainLayout());
	 }
};


#endif

