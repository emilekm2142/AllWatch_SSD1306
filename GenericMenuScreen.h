// GenericMenuScreen.h

#ifndef _GENERICMENUSCREEN_h
#define _GENERICMENUSCREEN_h

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
class GenericMenuScreen:CustomScreen
{
protected:
	int delay = 80;
	int maxOffset = 10;
	int step = 1;
public:
	int offset = 15;
	int currentIndex = 0;
	int optionsLength = 5;
	int spacing = 3;

	int lineHeight = 10;
	LinkedList<char*>* options;
	LinkedList<int*>* offsets = new LinkedList<int*>();
	LinkedList<std::function<void()>>* callbacks;
	
	UserInterfaceClass* UI;
	GenericMenuScreen(UserInterfaceClass* UI, LinkedList<char*> *options, LinkedList<std::function<void()>>* callbacks, int length) {
		this->options=options;
		this->callbacks = callbacks;
		offsets->add(new int(15));
		for (int i = 0; i < length-1; i++) offsets->add(new int(0));


	}
	virtual int CalculateHeight(Renderer& renderer) override {
		return renderer.GetScreenHeight();
	}
	virtual int CalculateWidth(Renderer& renderer) override {
		return renderer.GetScreenWidth();
	}
	virtual void Up(Renderer& renderer) override {
		if (*(offsets->get(currentIndex)) != 0) {
			auto a = new Animation(*(offsets->get(currentIndex)), 0, delay, -step);
			UI->RegisterAnimation(a);
		}

		currentIndex--;
		if (currentIndex < 0) currentIndex = optionsLength - 1;

		auto a2 = new Animation(*(offsets->get(currentIndex)), maxOffset, delay, step);
		UI->RegisterAnimation(a2);




	}
	virtual void Back(Renderer& r) override {
		UI->SetLayoutInFocues(*UI->GetMainLayout());
	}
	virtual void Down(Renderer& renderer) override {
		if (*(offsets->get(currentIndex)) != 0) {
			auto a = new Animation(*(offsets->get(currentIndex)), 0, delay, -step);
			UI->RegisterAnimation(a);
		}

		currentIndex++;
		if (currentIndex >= optionsLength) currentIndex = 0;

		auto a2 = new Animation(*(offsets->get(currentIndex)), maxOffset, delay, step);
		UI->RegisterAnimation(a2);






	}
	virtual void Ok(Renderer& renderer) override {


		auto o=callbacks->get(currentIndex);
		
		o();
		
	}



	virtual void Draw(Renderer& renderer) override {
		int y = offset;
		int x = 0;
		for (int i = 0; i < optionsLength; i++) {
			if (currentIndex != i)
				renderer.DrawString(x + GlobalX + *offsets->get(i), GlobalY + y, options->get(i));
			else {
				renderer.DrawString(x + GlobalX + *offsets->get(i) - 10, GlobalY + y, ">");
				renderer.DrawString(x + GlobalX + *offsets->get(i), GlobalY + y, options->get(i));
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
		offset = 15;
	}
	
};



#endif

