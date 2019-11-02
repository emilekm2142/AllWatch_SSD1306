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
class HomeScreen: public CustomScreen
{
 protected:
	 UserInterfaceClass* UI;
	 TimeKepper* tk;
 public:

	 HomeScreen(UserInterfaceClass& ui, TimeKepper* tk) {
		 UI = &ui;
		 this->tk = tk;
	 }
	 void Draw(Renderer& r) override {
		 int offset = 8;
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
#ifndef DISPLAY_SECONDS
		 snprintf_P(timestring,
			 20,
			 PSTR("%02u:%02u"),

			 tk->now.Hour(),
			 tk->now.Minute()
			 
		 );
#endif

#ifdef DISPLAY_SECONDS
		 snprintf_P(timestring,
			 20,
			 PSTR("%02u:%02u:%02u"),

			 tk->now.Hour(),
			 tk->now.Minute(),
			 tk->now.Second()

		 );
#endif
		 r.SetFont((uint8_t *) Orbitron_Medium_10);
		 r.DrawAlignedString(0, 0, datestring, r.GetScreenWidth(), r.Left);
	 	//TOdo: font musi byc tutaj mniejszy jak jest display_seconds
		 r.SetFont((uint8_t *)Orbitron_Medium_30);
		 r.DrawAlignedString(GlobalX+r.GetScreenWidth()/2, GlobalY+13+offset, timestring, r.GetScreenWidth(), r.Center);
		 r.SetFont((uint8_t *)Orbitron_Medium_10);
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

