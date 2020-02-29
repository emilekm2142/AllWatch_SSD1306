// GenericInputScreen.h

#pragma once
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
class GenericInputScreen: public CustomScreen
{
protected:

	bool headerScroll = true;
	int offset = 15;

public:
	const char* header;
	bool scrollable = true;
	Keyboard* kbrd;
	GenericInputScreen(UserInterfaceClass* UI, const char* header = nullptr) {
		this->UI = UI;
		this->header = header;
		kbrd = UI->GetKeyboard();
	}
	void OpenKeyboard() {
	
		UI->DisplayKeyboard();
	}
	UserInterfaceClass* UI;
	bool IsKeyboardActive() {
		return kbrd->visible;
	}
	char*  GetText() {
		return kbrd->target;
	}
	Keyboard* GetKeyboard() {
		return UI->GetKeyboard();
	}
	virtual void Back(Renderer& r) override {
		CustomScreen::Back(r);
	}
	virtual void Ok(Renderer& r) override {
		CustomScreen::Ok(r);
	}
	virtual void Down(Renderer& r) override {
		CustomScreen::Down(r);
	}
	virtual void Up(Renderer& r) override {
		CustomScreen::Up(r);
		
	}
	virtual void Draw(Renderer& r) override {
		//Serial.println("Draw call in generic text screen");
		if (this->header!=nullptr) r.DrawAlignedString(GlobalX + 0, GlobalY + offset, this->header, r.GetScreenWidth(), r.Left);
		//Serial.println(this->text);
		r.DrawAlignedString(GlobalX + 0, GlobalY + offset + 10, this->kbrd->target, r.GetScreenWidth(), r.Left);
	}
	virtual void DrawActiveIndicator(Renderer& renderer) override {
		
	}
};



