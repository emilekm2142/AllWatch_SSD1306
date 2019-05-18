// SettingsScreen.h

#ifndef _CONNECTINGSCREEN_h
#define _CONNECTINGSCREEN_h

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
class ConnectingScreen :public CustomScreen
{
protected:
	int offset = 15;

public:

	UserInterfaceClass* UI;
	SettingsManager* sm;
	ConnectingScreen(UserInterfaceClass*& UI, SettingsManager*& sm) {
		this->UI = UI;
		this->sm = sm;
	}

	virtual void Back(Renderer& r) override {
		sm->CloseSettings();
		Serial.println("back");
		UserInterface.ReturnToParentLayout();
	}
	virtual void Ok(Renderer& r) override {

	}
	virtual void Down(Renderer& r) override {
	//	auto a = new Animation(this->offset, offset - 10, 50, -1);
	//	UI->RegisterAnimation(a);
	}
	virtual void Up(Renderer& r) override {
		//auto a = new Animation(this->offset, offset + 10, 50, 1);
		//UI->RegisterAnimation(a);
	}
	virtual void Draw(Renderer& renderer) override {
		renderer.DrawAlignedString(renderer.GetScreenWidth() / 2, 50, "Connecting... \n this might take a few seconds", renderer.GetScreenWidth() / 2, renderer.Center);


	}
	virtual void OnGetInFocus(Renderer& r) override {
		offset = 15;
	}

};



#endif

