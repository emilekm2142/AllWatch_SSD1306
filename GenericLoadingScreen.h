// GenericLoadingScreen.h

#ifndef _GENERICLOADINGSCREEN_h
#define _GENERICLOADINGSCREEN_h

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
class GenericLoadingScreen:CustomScreen
{
 protected:


 public:
	 GenericLoadingScreen(UserInterfaceClass* UI) {
		 this->UI = UI;
	 }
	 void Ok(Renderer& r) override {
		 
	 }
	 void Down(Renderer& r) override {
		 
	 }
	 void Up(Renderer& r) override {

	 }
	 void Back(Renderer& r) override {

	 }
	void Draw(Renderer& r) override
	 {
		r.DrawAlignedString(r.GetHorizontalCenter() / 2, r.GetVerticalCenter() / 2, "Loading...", r.GetScreenWidth(), r.Center);
	 }
	
};


#endif

