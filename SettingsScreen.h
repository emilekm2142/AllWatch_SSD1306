// SettingsScreen.h

#ifndef _SETTINGSSCREEN_h
#define _SETTINGSSCREEN_h

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
#include "CustomScreen.h"
class SettingsScreen:public CustomScreen
{
 protected:
	 int offset = 15;

 public:
	 
	 UserInterfaceClass* UI;
	 SettingsManager* sm;
	 SettingsScreen(UserInterfaceClass*& UI, SettingsManager*& sm) {
		 this->UI = UI;
		 this->sm = sm;
	 }

	 virtual void Back(Renderer& r) override {
		
		 Serial.println("back");
		 UserInterface.ReturnToParentLayout();
	 }
	 virtual void Ok(Renderer& r) override {

	 }
	 virtual void Down(Renderer& r) override {
		 auto a = new Animation(this->offset, offset - 10, 50, -1);
		 UI->RegisterAnimation(a);
	 }
	 virtual void Up(Renderer& r) override {
		 auto a = new Animation(this->offset,offset+ 10, 50, 1);
		 UI->RegisterAnimation(a);
	 }
	 virtual void Draw(Renderer& r) override{
		 r.DrawAlignedString(GlobalX + 0, GlobalY + offset, "Connect to WiFi network named test and type 192.168.4.1 in your browser to access settings.", r.GetScreenWidth(), r.Left);

		
	 }
	 virtual void OnGetInFocus(Renderer& r) override {
		 offset = 15;
	 }

};



#endif

