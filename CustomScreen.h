// CustomScreen.h

#ifndef _CUSTOMSCREEN_h
#define _CUSTOMSCREEN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "LinkedList.h"
#include "Layout.h"
#include "Animation.h"
#include "Widget.h"
#include "UserInterface.h"
class CustomScreen:public Layout{
public:
	int bottomLineWidth = -1;
	UserInterfaceClass* UI=nullptr;
	virtual int CalculateHeight(Renderer& renderer) override {
		return height;
	}
	virtual int CalculateWidth(Renderer& renderer) override {
		return width;
	}
	virtual void GetInFocus(Renderer& renderer) override {
		GetInFocusExecutesBeforeGetInFocusInhertedFromCustomScreen(renderer);
		if (UI != nullptr) {
			bottomLineWidth = 0;
			auto a = new Animation(bottomLineWidth, renderer.GetScreenWidth(), 5, 5);
			Serial.println("Animation registered");
			UI->RegisterAnimation(a);
		}
		else {
			Serial.println("CustomScreen::GetInFocus: UI is null.");
		}
	}
	virtual void GetInFocusExecutesBeforeGetInFocusInhertedFromCustomScreen(Renderer& r) {

	}

	virtual void Up(Renderer& renderer) override {}
	virtual void Down(Renderer& renderer) override {}
	virtual void Ok(Renderer& renderer) override {}
	virtual void Back(Renderer& renderer) override {}
	
	
	virtual void DrawActiveIndicator(Renderer &renderer) override {
		//Serial.println("Drawing generic indicator");
		if (bottomLineWidth < 0) bottomLineWidth = renderer.GetScreenWidth();
		renderer.FillRectangle(renderer.GetHorizontalCenter() - bottomLineWidth/2, renderer.GetScreenHeight() - 1, 1,bottomLineWidth);
	}

	virtual void CalculateLayout(Renderer& renderer) override {
		
	}
};



#endif

