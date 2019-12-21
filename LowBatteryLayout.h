// LowBatteryLayout.h

#ifndef _LOWBATTERYLAYOUT_h
#define _LOWBATTERYLAYOUT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "LinkedList.h"
#include "Layout.h"
#include "Widget.h"
#include "TimeKepper.h"
#include "Animation.h"
#include "UserInterface.h"
#include "SettingsManager.h"

class LowBatteryLayout:public Layout
{
protected:


public:
	UserInterfaceClass* UI;

	virtual void Up(Renderer& renderer) override {

	}
	virtual void Down(Renderer& renderer) override {

	}
	virtual int CalculateHeight(Renderer& renderer) override {
		return 18;
	}
	virtual int CalculateWidth(Renderer& renderer) override {
		return renderer.GetScreenWidth();
	}
	virtual void Ok(Renderer& renderer) override {}
	virtual void Back(Renderer& renderer) override {}
	LowBatteryLayout(UserInterfaceClass* UI) {
		this->UI = UI;

	}


	void PlayShutdownAnimation(Renderer& renderer)
	{
		//https://www.youtube.com/watch?v=-MH6JZdGZcI

	}

	void Draw(Renderer& renderer) override
	{
		renderer.DrawXBM(renderer.GetHorizontalCenter() - 32, 0, 64, 64, UIAssets::btry_bits);
	}
};



#endif

