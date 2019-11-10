// GenericTextScreen.h

#ifndef _GENERICTEXTSCREEN_h
#define _GENERICTEXTSCREEN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "LinkedList.h"
#include "Layout.h"
#include "Widget.h"
#include "SettingsManager.h"
#include "Animation.h"
#include "UserInterface.h"
#include "StaticResources.h"
#include "SettingsScreen.h"
class GenericTextScreen:CustomScreen
{
 protected:
	 bool scrollable = true;
	 bool headerScroll = true;
	 int offset = 15;
	 
 public:
	 const char* text;
	 char* header;
	
	 GenericTextScreen(UserInterfaceClass* UI, const char* text, bool scrollable, char* header = nullptr, bool headerScroll=true ) {
		 this->text = text;
		 this -> scrollable = scrollable;
		 this->UI = UI;
		 this->header = header;
		 this->headerScroll = headerScroll;
	 }
	 UserInterfaceClass* UI;
	 virtual void Back(Renderer& r) override {
		 
		 UI->SetLayoutInFocues(*UI->GetMainLayout());
	 }
	 virtual void Ok(Renderer& r) override {

	 }
	 virtual void Down(Renderer& r) override {
		 if (scrollable) {
			 auto a = new Animation(this->offset, offset - 10, 50, -1);
			 UI->RegisterAnimation(a);
		 }
	 }
	 virtual void Up(Renderer& r) override {
		 if (scrollable) {
			 auto a = new Animation(this->offset, offset + 10, 50, 1);
			 UI->RegisterAnimation(a);
		 }
	 }
	 virtual void Draw(Renderer& r) override {
		 //Serial.println("Draw call in generic text screen");
		 //Serial.println(this->text);
		 r.DrawAlignedString(GlobalX + 0, GlobalY + offset, this->text, r.GetScreenWidth(), r.Left);
	 }
	 virtual void DrawActiveIndicator(Renderer &renderer) override {
		// Serial.println("Drawing generic text indicator");
		 renderer.FillRectangle(0, renderer.GetScreenHeight() - 1, 1, renderer.GetScreenWidth());
	 }

};


#endif

