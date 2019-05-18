// MainSlideLayout.h

#ifndef _MAINSLIDELAYOUT_h
#define _MAINSLIDELAYOUT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "LinkedList.h"
#include "Layout.h"
#include "Widget.h"
#include "Animation.h"
#include "UserInterface.h"
class MainSlideLayout :public Layout {
public:
	int currentIndex = 0;
	UserInterfaceClass* UI;
	virtual int CalculateHeight(Renderer& renderer) override {
		return renderer.GetScreenHeight();
	}
	virtual int CalculateWidth(Renderer& renderer) override {
		return width;
	}
	virtual void Up(Renderer& renderer) override {
		PrevScreen();
	}
	virtual void Down(Renderer& renderer) override {
		NextScreen();
	}
	virtual void Ok(Renderer& renderer) override {
		UI->SetLayoutInFocues( *(Layout*)GetChildren()->get(currentIndex) );
	}
	virtual void DrawActiveIndicator(Renderer &r) override {

	}
	virtual void Back(Renderer& r) override {}
	void NextScreen() {
		
		if (currentIndex == GetChildren()->size() - 1) { 
			currentIndex = 0;
		
		
			Animation* a = new Animation(GlobalX, 0, 1, 45);
			UI->RegisterAnimation(a);
		}
		else {
			
			//GlobalX -= 160;
			currentIndex++;
			Animation* a = new Animation(GlobalX, -currentIndex * 160, 1, -45);
			UI->RegisterAnimation(a);
		}
	}
	void PrevScreen() {
		Serial.print("Prev triggered: ");
		Serial.println(currentIndex);
		if (currentIndex == 0) {
			currentIndex = GetChildren()->size() - 1;
			Animation* a = new Animation(GlobalX, currentIndex * 160, 1, -45);
			UI->RegisterAnimation(a);
		}
		else {
			
			currentIndex--;
			Animation* a = new Animation(GlobalX, currentIndex * 160, 1, 45);
			UI->RegisterAnimation(a);
		}
		Serial.print("After prev triggered: ");
		Serial.println(currentIndex);
	}
	virtual void Draw(Renderer& renderer) override{
		int lowerBound = currentIndex - 1 >= 0 ? currentIndex - 1 : 0;
		int upperBound = currentIndex + 1 < GetChildren()->size() ? currentIndex + 1 : GetChildren()->size();
		for (int i = lowerBound; i < upperBound; i++) {
			if (i >= GetChildren()->size()) break;
			GetChildren()->get(i)->DrawWithState(renderer);
		}
	}
	virtual void CalculateLayout(Renderer& renderer) override {
		CalculateSizesPostOrderTreeWalk(renderer);
		//Serial.println("main calculate layout");
		int _x = 0;
		for (int i = 0; i < GetChildren()->size(); i++) {
			GetChildren()->get(i)->setX(_x);
			_x += 160;
		}
		ApplyGlobalCoordinatesPreorderTreeWalk(this,renderer);

	}
};



#endif

