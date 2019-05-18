// AppsMenu.h

#ifndef _APPSMENU_h
#define _APPSMENU_h

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
#include "BuiltInApplication.h"
#include "SettingsScreen.h"
class AppsMenu :public CustomScreen {
protected:
	int delay = 80;
	int maxOffset = 10;
	int step = 1;
public:
	int offset = 15;
	int currentIndex = 0;

	int spacing = 3;

	int lineHeight = 10;
	
	LinkedList<int*>* offsets = new LinkedList<int*>();
	

	
	UserInterfaceClass* UI;
	SettingsManager* settingsManager;
	AppsMenu(UserInterfaceClass* UI, SettingsManager* settingsManager) {
		this->UI = UI;
		this->settingsManager = settingsManager;
		this->offsets->add(new int(maxOffset));
		for (int i = 1; i < settingsManager->appsManager->builtInApps->size(); i++)
			this->offsets->add(new int(0));

	}
	virtual int CalculateHeight(Renderer& renderer) override {
		return renderer.GetScreenHeight();
	}
	virtual int CalculateWidth(Renderer& renderer) override {
		return renderer.GetScreenWidth();
	}
	virtual void Up(Renderer& renderer) override {
		if (*offsets->get(currentIndex) != 0) {
			auto a = new Animation(*offsets->get(currentIndex), 0, delay, -step);
			UI->RegisterAnimation(a);
		}

		currentIndex--;
		if (currentIndex < 0) currentIndex = settingsManager->appsManager->builtInApps->size() - 1;

		auto a2 = new Animation(*offsets->get(currentIndex), maxOffset, delay, step);
		UI->RegisterAnimation(a2);




	}
	virtual void DrawActiveIndicator(Renderer &renderer) override {
	
	}
	virtual void Back(Renderer& r) override {
		UI->SetLayoutInFocues(*UI->GetMainLayout());
	}
	virtual void Down(Renderer& renderer) override {
		if (*offsets->get(currentIndex) != 0) {
			auto a = new Animation(*offsets->get(currentIndex), 0, delay, -step);
			UI->RegisterAnimation(a);
		}

		currentIndex++;
		if (currentIndex >= settingsManager->appsManager->builtInApps->size()) currentIndex = 0;

		auto a2 = new Animation(*offsets->get(currentIndex), maxOffset, delay, step);
		UI->RegisterAnimation(a2);






	}
	virtual void Ok(Renderer& renderer) override {
		settingsManager->appsManager->builtInApps->get(currentIndex)->Open();
		
	}



	virtual void Draw(Renderer& renderer) override {
		int y = offset;
		int x = 0;
		for (int i = 0; i < settingsManager->appsManager->builtInApps->size(); i++) {
			if (currentIndex != i)
				renderer.DrawString(x + GlobalX + *offsets->get(i), GlobalY + y,settingsManager->appsManager->builtInApps->get(i)->name);
			else {
				renderer.DrawString(x + GlobalX + *offsets->get(i)- 10, GlobalY + y, ">");
				renderer.DrawString(x + GlobalX + *offsets->get(i), GlobalY + y, settingsManager->appsManager->builtInApps->get(i)->name);
			}
			y = y + lineHeight + spacing;
			if (y > renderer.GetScreenHeight()) {
				y = offset;
				x = x + renderer.GetScreenWidth() / 2;
			}
		}
	}
	virtual void CalculateLayout(Renderer& renderer) override {
		CalculateSizesPostOrderTreeWalk(renderer);
		ApplyGlobalCoordinatesPreorderTreeWalk(this, renderer);

	}
	virtual void OnGetInFocus(Renderer& r) override {
		
	}
};

#endif

