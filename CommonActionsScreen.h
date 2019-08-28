// CommonActionsScreen.h

#ifndef _COMMONACTIONSSCREEN_h
#define _COMMONACTIONSSCREEN_h

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
#include "ConnectingScreen.h"
#include "BuiltInApplication.h"
class CommonActionsScreen :public CustomScreen {
protected:
	int localDelay = 80;
	int maxOffset = 10;
	int step = 1;
public:
	int offset = 15;
	int currentIndex = 0;
	int optionsLength = 6;
	int spacing = 3;
	bool connecting = false;
	int lineHeight = 10;
	char* options[8] = { "Status","Settings","Sync time",  "Connect to WiFi", "Turn off", "Flashlight", "asdasf5", "asdasda" };
	int offsets[8] = { maxOffset,0,0,0,0,0,0,0 };
	
	
	UserInterfaceClass* UI;
	SettingsManager* settingsManager;
	CommonActionsScreen(UserInterfaceClass* UI, SettingsManager* settingsManager) {
		this->UI = UI;
		this->settingsManager = settingsManager;
	}
	virtual int CalculateHeight(Renderer& renderer) override {
		return renderer.GetScreenHeight();
	}
	virtual int CalculateWidth(Renderer& renderer) override {
		return renderer.GetScreenWidth();
	}
	virtual void Up(Renderer& renderer) override {
		if (offsets[currentIndex] != 0) {
			auto a = new Animation(offsets[currentIndex], 0, localDelay, -step);
			UI->RegisterAnimation(a);
		}

		currentIndex--;
		if (currentIndex < 0 ) currentIndex = optionsLength - 1;
		
			auto a2 = new Animation(offsets[currentIndex], maxOffset, localDelay, step);
			UI->RegisterAnimation(a2);
		

		
		
	}
	virtual void Back(Renderer& r) override {
		UI->SetLayoutInFocues(*UI->GetMainLayout());
	}
	virtual void Down(Renderer& renderer) override {
		if (offsets[currentIndex] != 0) {
			auto a = new Animation(offsets[currentIndex], 0, localDelay, -step);
			UI->RegisterAnimation(a);
		}

		currentIndex++;
		if (currentIndex >=optionsLength) currentIndex = 0;
			auto a2 = new Animation(offsets[currentIndex], maxOffset, localDelay, step);
			UI->RegisterAnimation(a2);
	}
	virtual void Ok(Renderer& renderer) override {
		
		Serial.printf("%d - currentIndex", currentIndex);
		switch (currentIndex) {
		case 0:
			Serial.println("gettin built in...");
			settingsManager->appsManager->getBuiltInApplicationByName("Status")->getApplication()->Open();
			break;
		case 1:
		{
			SettingsScreen* settingsScreen = new SettingsScreen(UI, settingsManager);
			settingsManager->OpenSettings();
			settingsScreen->UI = this->UI;
			UI->OpenChildLayout((Layout *)(settingsScreen));
		}
			break;
		case 2: {
			
			auto connectingScreen = new ConnectingScreen(UI, settingsManager, settingsManager->WiFiConnected() ? "syncing..." : "no internet!");
			UI->OpenChildLayout(connectingScreen);
			if (settingsManager->WiFiConnected()) settingsManager->SyncTime(); else  delay(1000);
			UI->ReturnToParentLayout();
			
			break;
		}
		case 3:{
			connecting = true;
			auto connectingScreen = new ConnectingScreen(UI, settingsManager);
			UI->OpenChildLayout(connectingScreen);
			if (!settingsManager->w->isConnected())
				bool conencted = settingsManager->wifiManager->ConnectToFirstFittingWiFiNetwork();
			else
				settingsManager->wifiManager->Disconnect();
			UI->ReturnToParentLayout();
			
			connecting = false;
			options[3] = settingsManager->WiFiConnected() ? (char*)"Disconnect Wifi" : (char*)"Connect to WiFi";


			
			//TODO: open settings info screen
		}
			break;
		case 4:
			renderer.DisableScreen();
			ESP.deepSleep(999999999 * 999999999U);
			break;
		case 5:
		{
			settingsManager->appsManager->getBuiltInApplicationByName("flashlight")->getApplication()->Open();
			break;
		}
		}
	}



	virtual void Draw(Renderer& renderer) override {
		options[3] = settingsManager->WiFiConnected() ? (char*)"Disconnect Wifi" : (char*)"Connect to WiFi";
		if (connecting) {
			renderer.DrawAlignedString(GlobalX + renderer.GetScreenWidth() / 2, 40, "Connecting...", renderer.GetScreenWidth(), renderer.Center);

			return;
		}
		int y = offset;
		int x = 0;
		for (int i = 0; i < optionsLength; i++) {
			if (currentIndex!=i)
			renderer.DrawString(x+GlobalX+offsets[i], GlobalY+y, options[i]);
			else {
				renderer.DrawString(x + GlobalX + offsets[i]-10, GlobalY + y, ">");
				renderer.DrawString(x + GlobalX + offsets[i], GlobalY + y, options[i]);
			}
			y = y + lineHeight + spacing;
			if (y > renderer.GetScreenHeight()) {
				y = offset;
				x = x + renderer.GetScreenWidth()/2;
			}
		}
	}
	virtual void CalculateLayout(Renderer& renderer) override {
		CalculateSizesPostOrderTreeWalk(renderer);
		ApplyGlobalCoordinatesPreorderTreeWalk(this, renderer);

	}
	virtual void OnGetInFocus(Renderer& r) override {
		offset = 15;
	}
};




#endif

