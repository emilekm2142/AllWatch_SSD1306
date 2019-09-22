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
	int scrollY = 0;
	UserInterfaceClass* UI;
	GenericMenuScreen(UserInterfaceClass* UI, LinkedList<char*> *options, LinkedList<std::function<void()>>* callbacks, int length) {
		this->options=options;
		this->UI = UI;
		this->callbacks = callbacks;
		offsets->add(new int(15));
		for (int i = 0; i < length-1; i++) offsets->add(new int(0));


	}
	GenericMenuScreen(UserInterfaceClass* UI) {
		options = new LinkedList<char*>();
		callbacks = new LinkedList<std::function<void()>>();
		this->UI = UI;


	}
	void ClearList() {
		//TOOD: deallocate the memory
		this->offsets->clear();
		this->options->clear();
		this->callbacks->clear();
	}
	void AddOption(char* name, std::function<void()> callback, bool copy=false) {
		if (offsets->size() == 0) {
			offsets->add(new int(15));
		}
		else {
			offsets->add(new int(0));
		}
		optionsLength++;
		if (copy) {
			Serial.println("Copy...");
			char* j = new char[strlen(name)+1];
			Serial.println("Copy...2");
			strcpy(j, name);
			Serial.println(j);
			Serial.println("Copy...3");
			options->add(j);
			Serial.println("Copy...4");
			callbacks->add(callback);
			
		}	
		else {
			options->add(name);
			callbacks->add(callback);
		\
		}

	
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
		int sign = -1;
		if (currentIndex < 0) { 
			currentIndex = options->size() - 1;
			sign = 1;
		}
		auto aScroll = new Animation(scrollY, currentIndex * 10, 1, sign*step);
		UI->RegisterAnimation(aScroll);
		auto a2 = new Animation(*(offsets->get(currentIndex)), maxOffset, delay, step);
		UI->RegisterAnimation(a2);




	}
	void setOnBackCallbackAction(std::function<void()> onBackCallback) {
		this->onBackCallback = onBackCallback;
	}
	virtual void Back(Renderer& r) override {
		if (this->onBackCallback != nullptr) {
			this->onBackCallback();
		}
		else {
			UI->SetLayoutInFocues(*UI->GetMainLayout());
		}
	}
	virtual void Down(Renderer& renderer) override {
		Serial.println("downnnn");
		if (*(offsets->get(currentIndex)) != 0) {
			auto a = new Animation(*(offsets->get(currentIndex)), 0, delay, -step);
			UI->RegisterAnimation(a);
			
		}

		currentIndex++;
		
		Serial.printf("%d == %d", currentIndex, options->size());
		int sign = 1;
		if (currentIndex >= options->size()) { currentIndex = 0; sign = -1; }
		
		auto aScroll = new Animation(scrollY,currentIndex * 10, 1,sign*step);
		UI->RegisterAnimation(aScroll);
		
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
		
		for (int i = 0; i < this->options->size(); i++) {
			if (currentIndex != i) {
				renderer.DrawString(x + GlobalX + *offsets->get(i), GlobalY + y -scrollY, options->get(i));
				//renderer.DrawString(5, 50, options->get(i));
			}
			else {
				//renderer.DrawString(5, 50, options->get(i));
				renderer.DrawString(x + GlobalX + *offsets->get(i) - 10, GlobalY + y-scrollY, ">");
				renderer.DrawString(x + GlobalX + *offsets->get(i), GlobalY + y-scrollY, options->get(i));
			}
			y = y + lineHeight + spacing;
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

