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
#include "Dependency.h"
#include "SettingsManager.h"
#include "DependenciesHolder.h"
#define net_wifi4_width 16
#define net_wifi4_height 16
static unsigned PROGMEM char net_wifi4_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0x08, 0x10, 0xe4, 0x27, 0x10, 0x08,
   0xc0, 0x03, 0x20, 0x04, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01,
   0xc0, 0x03, 0xc0, 0x03, 0x80, 0x01, 0x00, 0x00 };
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


	
	virtual void Draw(Renderer& renderer) override {
		renderer.FillRectangle(0, 0, 12, renderer.GetScreenWidth(), true);
		
		//renderer.DrawBitmap(0, 0, baseline_access_time_black_18dp_width, baseline_access_time_black_18dp_height, (const unsigned char*)UIAssets::baseline_access_time_black_18dp_bits);
		char buff[6];
		sprintf(buff, "%d", bm->GetBatteryLevel());
		renderer.DrawString(0, 0, buff);

		if (bm->IsGoingToSleep()) renderer.DrawString(0, 0, "zzz");
		renderer.DrawAlignedString(0, 0, UI->currentScreenName, renderer.GetScreenWidth(), renderer.Left);
		char datestring[20];

		snprintf_P(datestring,
			20,
			PSTR("%02u:%02u"),
			
			tk->now.Hour() ,
		
			tk->now.Minute()
			//,bm->GetBatteryLevel()
		
			
			);

		renderer.DrawAlignedString(renderer.GetScreenWidth() / 2, 0, datestring, renderer.GetScreenWidth() / 2, renderer.Center);
		if (sm->WiFiConnected()) renderer.DrawBitmap(renderer.GetScreenWidth() - 20, 2, 16, 16, net_wifi4_bits);
		
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

