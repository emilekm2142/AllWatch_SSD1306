// TopBar.h

#ifndef _TOPBAR_h
#define _TOPBAR_h

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
#include "StaticResources.h"
#include "SSD1306Fonts.h"

#include "SettingsManager.h"
#include "DependenciesHolder.h"

class TopBar :public Layout {
public:
	int currentIndex = 0;
	UserInterfaceClass* UI;
	BatteryManager* bm;
	TimeKepper* tk;
	SettingsManager* sm;
	//DependenciesHolder* dependencies;
	TopBar(UserInterfaceClass* UI, BatteryManager* bm, TimeKepper* tk, SettingsManager* sm) {
		this->UI = UI;
		this->bm = bm;
		this->tk = tk;
		this->sm = sm;
	}
	virtual int CalculateHeight(Renderer& renderer) override {
		return 18;
	}
	virtual int CalculateWidth(Renderer& renderer) override {
		return renderer.GetScreenWidth();
	}
	virtual void Up(Renderer& renderer) override {
		
	}
	virtual void Down(Renderer& renderer) override {
		
	}
	virtual void Ok(Renderer& renderer) override {}
	virtual void Back(Renderer& renderer) override {}

	void DrawBattery(Renderer& renderer)
	{
		const int batteryMiddleWidth = 15;
		const int batterySidesWidth = 1;
		int batBarWidth = (bm->GetBatteryPercentage() / 100.0) * batteryMiddleWidth;
		if (batBarWidth < 0) batBarWidth = batteryMiddleWidth;
		//sides
		renderer.FillRectangle(0, 4, 3, batterySidesWidth);
		renderer.FillRectangle(batteryMiddleWidth+batterySidesWidth, 4, 3, batterySidesWidth);
		//fill
		renderer.DrawRectangle(batterySidesWidth, 3, 5, batteryMiddleWidth);
		renderer.FillRectangle(batterySidesWidth, 3, 5, batBarWidth);
		char b[4];
		renderer.SetFont((uint8_t *)Orbitron_Medium_8);
		sprintf(b, "%d%%", abs(bm->GetBatteryPercentage()));
		renderer.DrawString(2*batterySidesWidth + batteryMiddleWidth+3, 3, b);
		renderer.SetFont((uint8_t *)Orbitron_Medium_10);
	}
	void DrawChargingIndicator(Renderer& renderer)
	{
		renderer.SetFont((uint8_t *)Orbitron_Medium_8);
		renderer.DrawString(0, 3, "Charging");
		renderer.SetFont((uint8_t *)Orbitron_Medium_10);
	}
	
	virtual void Draw(Renderer& renderer) override {

		renderer.FillRectangle(0, 0, 12, renderer.GetScreenWidth(), true);
		/*if (bm->isBeingCharged()) {
			DrawChargingIndicator(renderer);
		}else{
			DrawBattery(renderer);
		}*/
		DrawBattery(renderer);
		//renderer.DrawBitmap(0, 0, baseline_access_time_black_18dp_width, baseline_access_time_black_18dp_height, (const unsigned char*)UIAssets::baseline_access_time_black_18dp_bits);
		
		//if (bm->IsGoingToSleep()) renderer.DrawString(0, 0, "zzz");
		//renderer.DrawAlignedString(0, 0, UI->currentScreenName, renderer.GetScreenWidth(), renderer.Left);
		char datestring[20];

		snprintf_P(datestring,
			20,
			PSTR("%02u:%02u"),
			
			tk->now.Hour() ,
		
			tk->now.Minute()
			//,bm->GetBatteryLevel()
		
			
			);

		renderer.DrawAlignedString(renderer.GetScreenWidth() / 2, 0, datestring, renderer.GetScreenWidth() / 2, renderer.Center);
		if (sm->WiFiConnected()) renderer.DrawBitmap(renderer.GetScreenWidth() - 12, -1, 12, 12, UIAssets::baseline_wifi_black_18dp_bits);
		
		//renderer.DrawAlignedString(renderer.GetScreenWidth(), 0, , renderer.GetScreenWidth(), renderer.Right);
	}
	virtual void CalculateLayout(Renderer& renderer) override {
		CalculateSizesPostOrderTreeWalk(renderer);
		//Serial.println("main calculate layout");
		int _x = 0;
		for (int i = 0; i < GetChildren()->size(); i++) {
			GetChildren()->get(i)->setX(_x);
			_x += 160;
		}
		ApplyGlobalCoordinatesPreorderTreeWalk(this, renderer);

	}
};




#endif

