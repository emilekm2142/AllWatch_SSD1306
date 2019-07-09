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
#include <SSD1306Brzo.h> 
class MainSlideLayout :public Layout {
public:
	int stepSize = 25;
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
		//TOOD: tests
		DrawIndicators(r);
	}
	virtual void Back(Renderer& r) override {}
	void NextScreen() {
		
		if (currentIndex == GetChildren()->size() - 1) { 
			currentIndex = 0;
		
		
			Animation* a = new Animation(GlobalX, 0, 1, stepSize);
			UI->RegisterAnimation(a);
		}
		else {
			
			//GlobalX -= 160;
			currentIndex++;
			Animation* a = new Animation(GlobalX, -currentIndex * 160, 1, -stepSize);
			UI->RegisterAnimation(a);
		}
	}
	void PrevScreen() {
		Serial.print("Prev triggered: ");
		Serial.println(currentIndex);
		if (currentIndex == 0) {
			currentIndex = GetChildren()->size() - 1;
			Animation* a = new Animation(GlobalX, currentIndex * 160, 1, -stepSize);
			UI->RegisterAnimation(a);
		}
		else {
			
			currentIndex--;
			Animation* a = new Animation(GlobalX, currentIndex * 160, 1, stepSize);
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
	void DrawIndicators(Renderer& r) {
		//TODO: dodać abstrakcje do fontow

		int centerH = r.GetScreenWidth() / 2;
		int offsetFromCenterH = 5;
		int indicatorY = r.GetScreenHeight() - 7;
		int childrenSize = GetChildren()->size();
		bool isEven = childrenSize % 2 == 0;
		int centerPoint = childrenSize / 2 + 1;

		int extraHOffset = isEven ? 5:0;

		for (int i = 0; i < childrenSize; i++) {
			if (i == currentIndex)
				r.SetFont((uint8_t *)ArialMT_Plain_24);
			else
				r.SetFont((uint8_t *)ArialMT_Plain_10);

			int positionsLeftToCenterPoint = i - centerPoint;
			r.DrawString(centerH + positionsLeftToCenterPoint * offsetFromCenterH, indicatorY, ".");
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

