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
		
		renderer.FillRectangle(0, 0, 18, renderer.GetScreenWidth(), true);
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
		if (sm->WiFiConnected()) renderer.DrawAlignedString(renderer.GetScreenWidth() , 0, "WiFi", renderer.GetScreenWidth() / 2, renderer.Right);
		
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

